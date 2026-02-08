import os
import re
import json
import time
from dataclasses import dataclass, asdict
from typing import Optional
from pathlib import Path
from collections import defaultdict
from datetime import datetime, timedelta, timezone

from googleapiclient.discovery import build
from googleapiclient.errors import HttpError


# ----------------------------
# CONFIG
# ----------------------------

OUTPUT_DIR = Path("output")

HITS_JSONL = OUTPUT_DIR / "timeline_hits.jsonl"
SUMMARY_JSON = OUTPUT_DIR / "timeline_summary.json"
STATS_JSON = OUTPUT_DIR / "run_stats.json"

PUBLISHED_AFTER = "2025-01-05T00:00:00Z"
PUBLISHED_BEFORE = "2025-02-02T00:00:00Z"

EVENT_WINDOW_HOURS = 48  # <-- HARD 48h WINDOW

# 🔥 IGNITION TIME OVERRIDES (UTC)
FIRE_START_OVERRIDE = {
    "Eaton Fire": datetime(2025, 1, 8, 2, 11, tzinfo=timezone.utc),
    "Palisades Fire": datetime(2025, 1, 7, 18, 30, tzinfo=timezone.utc),
}

QUERIES = [
    '"Eaton Fire" Altadena',
    '"Eaton Fire" Pasadena',
    '"Palisades Fire" "Pacific Palisades"',
    '"Pacific Palisades" evacuation',
    '"Altadena" evacuation',
]

CLUSTER_WINDOW_MIN = 30  # ±30 minutes


# ----------------------------
# REGEX
# ----------------------------

DETECTION_RE = re.compile(r"smelled smoke|saw smoke|woke up|noticed", re.I)
NOTIFY_RE = re.compile(r"alert|emergency alert|siren|news|tv|radio", re.I)
PREP_RE = re.compile(r"packed|packing|get(ting)? ready|grabbed|prepared", re.I)
EVAC_RE = re.compile(r"evacuat|left at|in the car|traffic|got out", re.I)
ANCHOR_RE = re.compile(
    r"eaton|altadena|pasadena|palisades|pacific palisades", re.I
)


# ----------------------------
# HELPERS
# ----------------------------

def ensure_output_dir():
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)


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


def infer_fire(text: str, query: str) -> str:
    t = (text + " " + query).lower()
    if "eaton" in t or "altadena" in t:
        return "Eaton Fire"
    if "palisades" in t:
        return "Palisades Fire"
    return "Unknown"


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


def parse_time(ts: str) -> datetime:
    return datetime.fromisoformat(ts.replace("Z", "+00:00"))


# ----------------------------
# DATA STRUCTURE
# ----------------------------

@dataclass
class TimelineHit:
    fire_name: str
    stage: str
    published_at: str
    raw_confidence: float
    final_confidence: float = 0.0
    like_count: int = 0
    text: str = ""


# ----------------------------
# MAIN
# ----------------------------

def main():
    ensure_output_dir()
    yt = youtube_client()

    timeline = defaultdict(list)
    stats = {
        "hits_saved": 0,
        "videos_skipped_comments_disabled": 0
    }

    # ----------------------------
    # SCRAPE + HARD WINDOW FILTER
    # ----------------------------

    for q in QUERIES:
        search = call_with_retries(lambda: yt.search().list(
            part="snippet",
            q=q,
            type="video",
            maxResults=20,
            publishedAfter=PUBLISHED_AFTER,
            publishedBefore=PUBLISHED_BEFORE
        ).execute())

        for item in search.get("items", []):
            vid = item["id"]["videoId"]

            try:
                resp = call_with_retries(lambda: yt.commentThreads().list(
                    part="snippet",
                    videoId=vid,
                    maxResults=100,
                    order="time",
                    textFormat="plainText"
                ).execute())
            except HttpError:
                stats["videos_skipped_comments_disabled"] += 1
                continue

            for c in resp.get("items", []):
                sn = c["snippet"]["topLevelComment"]["snippet"]
                t = parse_time(sn.get("publishedAt"))
                text = sn.get("textOriginal", "")
                stage = classify_stage(text)
                if not stage:
                    continue

                fire = infer_fire(text, q)
                if fire not in FIRE_START_OVERRIDE:
                    continue  # safety

                fire_start = FIRE_START_OVERRIDE[fire]
                fire_end = fire_start + timedelta(hours=EVENT_WINDOW_HOURS)

                if not (fire_start <= t <= fire_end):
                    continue  # HARD FILTER HERE

                likes = int(sn.get("likeCount", 0))
                anchored = bool(ANCHOR_RE.search(text))

                raw = 1.0 + min(likes / 10.0, 3.0) + (1.0 if anchored else 0.0)

                hit = TimelineHit(
                    fire_name=fire,
                    stage=stage,
                    published_at=sn.get("publishedAt"),
                    raw_confidence=round(raw, 2),
                    like_count=likes,
                    text=text
                )

                timeline[fire].append(hit)
                stats["hits_saved"] += 1

    # ----------------------------
    # CONFIDENCE COMPUTATION
    # ----------------------------

    for fire, hits in timeline.items():
        earliest = min(parse_time(h.published_at) for h in hits)

        stages = defaultdict(list)
        for h in hits:
            stages[h.stage].append(h)

        for stage, items in stages.items():
            for h in items:
                t = parse_time(h.published_at)
                days_late = (t - earliest).total_seconds() / 86400

                conf = min(h.raw_confidence / 5.0, 1.0)
                conf = max(conf - 0.1 * days_late, 0.0)

                cluster = sum(
                    1 for x in items
                    if abs((parse_time(x.published_at) - t).total_seconds())
                    <= CLUSTER_WINDOW_MIN * 60
                )

                h.final_confidence = round(min(conf + 0.05 * cluster, 1.0), 3)

    # ----------------------------
    # WRITE FILTERED HITS
    # ----------------------------

    with HITS_JSONL.open("w", encoding="utf-8") as f:
        for hits in timeline.values():
            for h in hits:
                f.write(json.dumps(asdict(h), ensure_ascii=False) + "\n")

    # ----------------------------
    # BUILD SUMMARY
    # ----------------------------

    summary = {}

    for fire, hits in timeline.items():
        stages = defaultdict(list)
        for h in hits:
            stages[h.stage].append(h)

        best = {}
        t0 = min(parse_time(h.published_at) for h in hits)

        for stage in ("detection", "notification", "preparation", "evacuation"):
            items = [
                h for h in stages.get(stage, [])
                if parse_time(h.published_at) >= t0
            ]
            if not items:
                continue

            chosen = max(items, key=lambda x: x.final_confidence)
            best[stage] = chosen
            t0 = parse_time(chosen.published_at)

        def diff(a, b):
            if a in best and b in best:
                return round(
                    (parse_time(best[b].published_at)
                     - parse_time(best[a].published_at)).total_seconds() / 60, 1
                )
            return None

        summary[fire] = {
            "highlighted_timestamps": {
                f"T_{k}": v.published_at for k, v in best.items()
            },
            "elapsed_minutes": {
                "T_notification - T_detection": diff("detection", "notification"),
                "T_preparation - T_notification": diff("notification", "preparation"),
                "T_evacuation - T_preparation": diff("preparation", "evacuation"),
                "T_evacuation - T_detection": diff("detection", "evacuation"),
            },
            "final_confidence_by_stage": {
                f"T_{k}": v.final_confidence for k, v in best.items()
            }
        }

    with SUMMARY_JSON.open("w", encoding="utf-8") as f:
        json.dump(summary, f, indent=2)

    with STATS_JSON.open("w", encoding="utf-8") as f:
        json.dump(stats, f, indent=2)

    print("Done.")
    print(json.dumps(summary, indent=2))


if __name__ == "__main__":
    main()