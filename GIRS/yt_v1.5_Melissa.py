import os
import re
import json
import time
from dataclasses import dataclass, asdict
from typing import Optional
from pathlib import Path
from datetime import datetime, timedelta

from googleapiclient.discovery import build
from googleapiclient.errors import HttpError


# ============================
# CONFIG
# ============================

OUTPUT_DIR = Path("output/v1.5")
OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

HITS_JSONL = OUTPUT_DIR / "timeline_hits_v1.5.jsonl"

PUBLISHED_AFTER = "2025-01-05T00:00:00Z"
PUBLISHED_BEFORE = "2025-02-02T00:00:00Z"

QUERIES = [
    '"Eaton Fire" Altadena',
    '"Eaton Fire" Pasadena',
    '"Palisades Fire" "Pacific Palisades"',
    '"Pacific Palisades" evacuation',
    '"Altadena" evacuation',
]

SEARCH_MAX_RESULTS = 20
MIN_COMMENTS_PER_VIDEO = 30


# ============================
# REGEX
# ============================

NOTIFY_RE = re.compile(
    r"\bgo order\b|\bevac order\b|\bmandatory evacuation\b|\breverse 911\b|"
    r"\bwireless emergency alert\b|\bWEA\b|\bnixle\b|\balert la\b|\btext alert\b|\bwatch duty\b",
    re.I
)

EVAC_RE = re.compile(
    r"(we|i|my|our)\s+(left|evacuated|got out|drove|fled)|"
    r"evac(uat(e|ed|ing)|uatd|uation)|"
    r"gridlock|stuck on",
    re.I
)

DETECTION_RE = re.compile(
    r"\bash|\bglow\b|\borange sky\b|\bembers\b|smelled smoke|saw smoke",
    re.I
)

PREP_RE = re.compile(
    r"loaded the car|grabbed the pets|packed meds|gassed up|packed",
    re.I
)

LOCATION_RE = re.compile(
    r"(altadena|pasadena|pacific palisades|malibu|pch|sunset blvd)",
    re.I
)

EXPLICIT_TIME_RE = re.compile(
    r"(?:at|around|about)\s*(\d{1,2})(?::(\d{2}))?\s*(am|pm)",
    re.I
)

RELATIVE_TIME_RE = re.compile(
    r"(\d+)\s*(minutes?|mins?|hours?|hrs?)\s*(before|earlier)",
    re.I
)


# ============================
# HELPERS
# ============================

def youtube_client():
    key = os.getenv("YOUTUBE_API_KEY")
    if not key:
        raise SystemExit("Missing YOUTUBE_API_KEY")
    return build("youtube", "v3", developerKey=key)


def call_with_retries(fn, max_tries=4):
    delay = 1
    for _ in range(max_tries):
        try:
            return fn()
        except HttpError as e:
            if getattr(e.resp, "status", None) in (429, 500, 502, 503, 504):
                time.sleep(delay)
                delay *= 2
                continue
            raise
    raise RuntimeError("API failed after retries")


def parse_time(ts: str) -> datetime:
    return datetime.fromisoformat(ts.replace("Z", "+00:00"))


def classify_stage(text: str) -> Optional[str]:
    if EVAC_RE.search(text):
        return "evacuation"
    if PREP_RE.search(text):
        return "preparation"
    if NOTIFY_RE.search(text):
        return "notification"
    if DETECTION_RE.search(text):
        return "detection"
    return None


def infer_fire_from_text(text: str, query: str) -> str:
    t = (text + " " + query).lower()

    if "palisades" in t or "pacific palisades" in t or "malibu" in t or "pch" in t:
        return "Palisades Fire"

    if "eaton" in t or "altadena" in t or "pasadena" in t:
        return "Eaton Fire"

    return "Unknown Fire"


def infer_event_time(text: str, published_at: datetime):
    t = text.lower()

    m = EXPLICIT_TIME_RE.search(t)
    if m:
        hour = int(m.group(1))
        minute = int(m.group(2) or 0)
        ampm = m.group(3).lower()

        if ampm == "pm" and hour != 12:
            hour += 12
        if ampm == "am" and hour == 12:
            hour = 0

        candidate = published_at.replace(hour=hour, minute=minute, second=0)
        if candidate > published_at:
            candidate -= timedelta(days=1)

        return candidate.isoformat(), "explicit_time"

    m = RELATIVE_TIME_RE.search(t)
    if m:
        value = int(m.group(1))
        unit = m.group(2)
        delta = timedelta(minutes=value if "min" in unit else value * 60)
        return (published_at - delta).isoformat(), "relative_time"

    return None, "none"


def fetch_all_comments(yt, video_id):
    comments = []
    token = None

    while True:
        resp = call_with_retries(lambda: yt.commentThreads().list(
            part="snippet,replies",
            videoId=video_id,
            maxResults=100,
            pageToken=token,
            order="time",
            textFormat="plainText"
        ).execute())

        for item in resp.get("items", []):
            item["is_reply"] = False
            comments.append(item)

            
            if "replies" in item:
                for reply in item["replies"]["comments"]:
                    comment = {"snippet": {"topLevelComment": reply}}
                    comment["is_reply"] = True
                    comments.append(comment)
        
        token = resp.get("nextPageToken")
        if not token:
            break

    return comments


# ============================
# DATA STRUCTURE
# ============================

@dataclass
class TimelineHit:
    fire_name: str
    stage: str
    event_time: Optional[str]
    event_time_type: str
    published_at: str
    location_text: Optional[str]
    video_url: str
    comment_url: str
    video_comment_count: int
    is_reply: bool
    text: str


# ============================
# MAIN
# ============================

def main():
    yt = youtube_client()
    hits = []

    seen_comments = set()
    duplicate_count = 0

    for q in QUERIES:
        search = call_with_retries(lambda: yt.search().list(
            part="snippet",
            q=q,
            type="video",
            maxResults=SEARCH_MAX_RESULTS,
            publishedAfter=PUBLISHED_AFTER,
            publishedBefore=PUBLISHED_BEFORE
        ).execute())

        video_ids = [i["id"]["videoId"] for i in search.get("items", [])]
        if not video_ids:
            continue

        stats = call_with_retries(lambda: yt.videos().list(
            part="statistics",
            id=",".join(video_ids)
        ).execute())

        videos = []
        for v in stats["items"]:
            count = int(v["statistics"].get("commentCount", 0))
            if count >= MIN_COMMENTS_PER_VIDEO:
                videos.append((v["id"], count))

        videos.sort(key=lambda x: x[1], reverse=True)

        for vid, comment_count in videos:
            comments = fetch_all_comments(yt, vid)
            video_url = f"https://www.youtube.com/watch?v={vid}"

            for c in comments:
                comment_id = c["snippet"]["topLevelComment"]["id"]
                key = (vid, comment_id)

                if key in seen_comments:
                    duplicate_count += 1
                    continue

                seen_comments.add(key)

                sn = c["snippet"]["topLevelComment"]["snippet"]
                text = sn.get("textOriginal", "")

                stage = classify_stage(text)
                if not stage:
                    continue

                fire = infer_fire_from_text(text, q)

                published_at = parse_time(sn["publishedAt"])
                event_time, event_time_type = infer_event_time(text, published_at)

                loc = LOCATION_RE.search(text)
                location_text = loc.group(0) if loc else None

                comment_url = f"{video_url}&lc={comment_id}"

                hits.append(TimelineHit(
                    fire_name=fire,
                    stage=stage,
                    event_time=event_time,
                    event_time_type=event_time_type,
                    published_at=sn["publishedAt"],
                    location_text=location_text,
                    video_url=video_url,
                    comment_url=comment_url,
                    video_comment_count=comment_count,
                    is_reply = c.get("is_reply", False),
                    text=text
                ))

    with HITS_JSONL.open("w", encoding="utf-8") as f:
        for h in hits:
            f.write(json.dumps(asdict(h), ensure_ascii=False) + "\n")

    print("Scrape complete.")
    print(f"Total hits saved   : {len(hits)}")
    print(f"Duplicates skipped : {duplicate_count}")
    print(f"Output file        : {HITS_JSONL.resolve()}")


if __name__ == "__main__":
    main()
