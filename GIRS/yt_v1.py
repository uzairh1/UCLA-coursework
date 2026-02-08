import os

import re

import json

import csv

import time

from dataclasses import dataclass, asdict

from typing import Dict, List, Optional, Tuple

from pathlib import Path



from googleapiclient.discovery import build

from googleapiclient.errors import HttpError





# ----------------------------

# CONFIG

# ----------------------------



OUTPUT_DIR = Path("output")

VIDEOS_JSONL = OUTPUT_DIR / "videos.jsonl"

HITS_JSONL = OUTPUT_DIR / "comments_hits.jsonl"

HITS_CSV = OUTPUT_DIR / "comments_hits.csv"

STATS_JSON = OUTPUT_DIR / "run_stats.json"



# Jan 7–31, 2025 (±2 days)

PUBLISHED_AFTER = "2025-01-05T00:00:00Z"

PUBLISHED_BEFORE = "2025-02-02T00:00:00Z"



# Quota control

MAX_VIDEO_PAGES_PER_QUERY = 2        # each page is expensive (search.list)

MAX_COMMENTS_PER_VIDEO = 500         # cap comment pagination



# Queries

TIER_1_QUERIES = [

    '"Eaton Fire" Altadena',

    '"Eaton Fire" Pasadena',

    '"Palisades Fire" "Pacific Palisades"',

    '"Pacific Palisades" evacuation',

    '"Altadena" evacuation',

    '"Eaton Fire" "smelled smoke"',

    '"Palisades Fire" "smelled smoke"',

]



RUN_TIER_1_ONLY = True





# ----------------------------

# KEYWORD FILTERS

# ----------------------------



DETECTION_PHRASES = [

    r"smelled smoke",

    r"saw smoke",

    r"woke up",

    r"first saw",

    r"noticed",

    r"looked outside",

    r"sirens?",

    r"fire trucks?",

    r"alert(?:ed)?",

]



EVAC_PHRASES = [

    r"evacuat(?:e|ed|ing|ion)",

    r"\bleft at\b",

    r"\bwe left\b",

    r"\bpacked\b",

    r"\bgot out\b",

    r"\bhad to leave\b",

    r"\bin the car\b",

    r"\btraffic\b",

    r"\bstuck\b",

    r"\broad closed\b",

]



EVENT_ANCHORS = [

    r"eaton",

    r"altadena",

    r"pasadena",

    r"palisades",

    r"pacific palisades",

    r"topanga",

    r"sunset",

]



DETECTION_RE = re.compile("|".join(f"(?:{p})" for p in DETECTION_PHRASES), re.IGNORECASE)

EVAC_RE = re.compile("|".join(f"(?:{p})" for p in EVAC_PHRASES), re.IGNORECASE)

ANCHOR_RE = re.compile("|".join(f"(?:{p})" for p in EVENT_ANCHORS), re.IGNORECASE)



TIME_RE = re.compile(r"\b(?:at\s*)?(\d{1,2})(?::(\d{2}))?\s*(am|pm)?\b", re.IGNORECASE)





def extract_times(text: str) -> List[str]:

    out = []

    for m in TIME_RE.finditer(text):

        h = m.group(1)

        mm = m.group(2) or "00"

        ampm = (m.group(3) or "").lower().strip()

        out.append(f"{h}:{mm}{(' ' + ampm) if ampm else ''}".strip())

    # de-dupe while keeping order

    seen = set()

    deduped = []

    for t in out:

        if t not in seen:

            seen.add(t)

            deduped.append(t)

    return deduped





# ----------------------------

# DATA STRUCTURES

# ----------------------------



@dataclass

class VideoRow:

    query: str

    video_id: str

    title: str

    channel_title: str

    published_at: str

    url: str





@dataclass

class CommentHitRow:

    query: str

    video_id: str

    video_url: str

    comment_id: str

    author_display_name: str

    published_at: str

    like_count: int

    text: str  # <-- FULL COMMENT CONTENT

    matched_detection: bool

    matched_evac: bool

    matched_anchor: bool

    extracted_times: List[str]





# ----------------------------

# IO HELPERS

# ----------------------------



def ensure_output_dir() -> None:

    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)





def append_jsonl(path: Path, row: Dict) -> None:

    with path.open("a", encoding="utf-8") as f:

        f.write(json.dumps(row, ensure_ascii=False) + "\n")





def append_csv(path: Path, row: Dict, fieldnames: List[str]) -> None:

    exists = path.exists()

    with path.open("a", encoding="utf-8", newline="") as f:

        w = csv.DictWriter(f, fieldnames=fieldnames, quoting=csv.QUOTE_ALL, extrasaction="ignore")

        if not exists:

            w.writeheader()

        w.writerow(row)





# ----------------------------

# API HELPERS

# ----------------------------



def youtube_client():

    key = os.getenv("YOUTUBE_API_KEY")

    if not key:

        raise SystemExit('Missing YOUTUBE_API_KEY. Do: export YOUTUBE_API_KEY="YOUR_KEY"')

    return build("youtube", "v3", developerKey=key)





def call_with_retries(fn, max_tries: int = 4):

    """

    Retry for transient HttpError (429/5xx) with backoff.

    """

    delay = 1.0

    for attempt in range(1, max_tries + 1):

        try:

            return fn()

        except HttpError as e:

            status = getattr(e.resp, "status", None)

            if status in (429, 500, 502, 503, 504):

                time.sleep(delay)

                delay *= 2

                continue

            raise

    raise RuntimeError("API failed after retries")





def search_videos(youtube, query: str) -> List[VideoRow]:

    videos: List[VideoRow] = []

    page_token: Optional[str] = None



    for _ in range(MAX_VIDEO_PAGES_PER_QUERY):

        def do():

            return youtube.search().list(

                part="snippet",

                q=query,

                type="video",

                maxResults=50,

                pageToken=page_token,

                publishedAfter=PUBLISHED_AFTER,

                publishedBefore=PUBLISHED_BEFORE,

                order="date",

            ).execute()



        try:

            resp = call_with_retries(do)

        except HttpError:

            break



        for item in resp.get("items", []):

            vid = item["id"]["videoId"]

            sn = item["snippet"]

            videos.append(VideoRow(

                query=query,

                video_id=vid,

                title=sn.get("title", ""),

                channel_title=sn.get("channelTitle", ""),

                published_at=sn.get("publishedAt", ""),

                url=f"https://www.youtube.com/watch?v={vid}",

            ))



        page_token = resp.get("nextPageToken")

        if not page_token:

            break



    return videos





def fetch_comment_hits_for_video(youtube, query: str, video: VideoRow) -> Tuple[int, int]:

    page_token: Optional[str] = None

    fetched = 0

    pages = 0

    hits = 0



    while fetched < MAX_COMMENTS_PER_VIDEO:

        max_results = min(100, MAX_COMMENTS_PER_VIDEO - fetched)



        def do():

            return youtube.commentThreads().list(

                part="snippet",

                videoId=video.video_id,

                maxResults=max_results,

                pageToken=page_token,

                textFormat="plainText",

                order="time",

            ).execute()



        try:

            resp = call_with_retries(do)

        except HttpError:

            # comments disabled / restricted / etc.

            break



        items = resp.get("items", [])

        if not items:

            break



        pages += 1



        for it in items:

            top = it["snippet"]["topLevelComment"]["snippet"]



            # Prefer textOriginal, fall back to textDisplay

            text = top.get("textOriginal") or top.get("textDisplay") or ""

            author = top.get("authorDisplayName") or ""

            published_at = top.get("publishedAt") or ""

            like_count = int(top.get("likeCount") or 0)

            comment_id = it["snippet"]["topLevelComment"]["id"]



            matched_detection = bool(DETECTION_RE.search(text))

            matched_evac = bool(EVAC_RE.search(text))

            matched_anchor = bool(ANCHOR_RE.search(text))



            if matched_detection or matched_evac:

                extracted = extract_times(text)

                row = CommentHitRow(

                    query=query,

                    video_id=video.video_id,

                    video_url=video.url,

                    comment_id=comment_id,

                    author_display_name=author,

                    published_at=published_at,

                    like_count=like_count,

                    text=text,

                    matched_detection=matched_detection,

                    matched_evac=matched_evac,

                    matched_anchor=matched_anchor,

                    extracted_times=extracted,

                )



                row_dict = asdict(row)

                append_jsonl(HITS_JSONL, row_dict)



                # Flatten extracted_times for CSV

                row_csv = dict(row_dict)

                row_csv["extracted_times"] = "; ".join(row_csv.get("extracted_times", []) or [])

                append_csv(HITS_CSV, row_csv, fieldnames=list(row_csv.keys()))

                hits += 1



            fetched += 1

            if fetched >= MAX_COMMENTS_PER_VIDEO:

                break



        page_token = resp.get("nextPageToken")

        if not page_token:

            break



    return pages, hits





# ----------------------------

# MAIN

# ----------------------------



def main():

    ensure_output_dir()



    yt = youtube_client()



    queries = list(TIER_1_QUERIES)

    if not RUN_TIER_1_ONLY:

        # add more queries here if you want

        pass



    stats = {

        "published_after": PUBLISHED_AFTER,

        "published_before": PUBLISHED_BEFORE,

        "queries_run": 0,

        "videos_found_total": 0,

        "unique_videos_total": 0,

        "comment_pages_fetched_total": 0,

        "comment_hits_saved_total": 0,

        "outputs": {

            "videos_jsonl": str(VIDEOS_JSONL),

            "hits_jsonl": str(HITS_JSONL),

            "hits_csv": str(HITS_CSV),

            "stats_json": str(STATS_JSON),

        }

    }



    seen = set()



    for q in queries:

        stats["queries_run"] += 1

        vids = search_videos(yt, q)

        stats["videos_found_total"] += len(vids)



        for v in vids:

            if v.video_id in seen:

                continue

            seen.add(v.video_id)



            append_jsonl(VIDEOS_JSONL, asdict(v))



            pages, hits = fetch_comment_hits_for_video(yt, q, v)

            stats["comment_pages_fetched_total"] += pages

            stats["comment_hits_saved_total"] += hits



    stats["unique_videos_total"] = len(seen)



    with STATS_JSON.open("w", encoding="utf-8") as f:

        json.dump(stats, f, indent=2)



    print("Done.")

    print(f"Videos: {stats['unique_videos_total']} -> {VIDEOS_JSONL}")

    print(f"Hits JSONL: {stats['comment_hits_saved_total']} -> {HITS_JSONL}")

    print(f"Hits CSV: {stats['comment_hits_saved_total']} -> {HITS_CSV}")

    print(f"Stats: -> {STATS_JSON}")





if __name__ == "__main__":

    main()
