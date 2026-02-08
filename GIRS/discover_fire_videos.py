import os
import subprocess
import re
from datetime import datetime
from googleapiclient.discovery import build
import pandas as pd

# ============================
# CONFIG
# ============================

YOUTUBE_API_KEY = os.getenv("YOUTUBE_API_KEY")

FIRE_START_DATES = {
    "Eaton Fire": "2025-01-07",
    "Palisades Fire": "2025-01-07"
}

SEARCH_QUERIES = [
    "Eaton Fire",
    "Eaton Fire evacuation",
    "Eaton Fire alert",
    "Altadena Eaton Fire",
    "Palisades Fire",
    "Palisades Fire evacuation",
    "Palisades Fire alert",
    "Pacific Palisades fire"
]

WINDOW_DAYS = 4
MAX_VIDEOS_PER_QUERY = 60
OUTPUT_PATH = "partial_search.xlsx"

TOPIC_KEYWORDS = {
    "Evacuation": [
        "evacuate", "evacuation", "flee", "leave", "escape",
        "get out", "forced to leave", "mandatory order"
    ],
    "Notification": [
        "alert", "warning", "notification", "text message",
        "press conference", "officials said", "authorities said"
    ],
    "Detection": [
        "detected", "camera", "ignition", "spark",
        "first spotted", "initial fire", "power line"
    ],
    "Preparation": [
        "prepare", "packed", "staged", "ready",
        "getting ready", "preparing", "bags packed"
    ]
}

# ============================
# TRANSCRIPT CLEANING
# ============================

def clean_vtt_text(vtt_text):
    """
    Convert WebVTT captions into clean spoken transcript text.
    """
    lines = vtt_text.splitlines()
    cleaned = []

    for line in lines:
        line = line.strip()

        # Skip empty lines
        if not line:
            continue

        # Skip timestamps
        if "-->" in line:
            continue

        # Skip WEBVTT header
        if line.startswith("WEBVTT"):
            continue

        # Skip CSS / cue blocks
        if line.startswith("::cue") or line.startswith("{") or line.startswith("}"):
            continue

        # Remove HTML / <c> tags
        line = re.sub(r"<[^>]+>", "", line)

        # Skip leftover styling rules
        if ":" in line and line.lower().startswith("color"):
            continue

        cleaned.append(line)

    return " ".join(cleaned)

# ============================
# HELPERS
# ============================

def within_window(published_at):
    pub = datetime.strptime(published_at[:10], "%Y-%m-%d")
    for d in FIRE_START_DATES.values():
        center = datetime.strptime(d, "%Y-%m-%d")
        if abs((pub - center).days) <= WINDOW_DAYS:
            return True
    return False

def assign_topics(text):
    if not text:
        return []
    text = text.lower()
    topics = []
    for topic, terms in TOPIC_KEYWORDS.items():
        if any(t in text for t in terms):
            topics.append(topic)
    return topics

def get_auto_transcript(video_id):
    """
    Fetch and clean auto captions using yt-dlp.
    """
    print(f"    yt-dlp → fetching captions for {video_id}")

    cmd = [
        "yt-dlp",
        "--skip-download",
        "--write-auto-subs",
        "--sub-lang", "en",
        "--sub-format", "vtt",
        "-o", video_id,
        f"https://www.youtube.com/watch?v={video_id}"
    ]

    try:
        subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            check=True
        )
    except subprocess.CalledProcessError:
        return None

    vtt_file = f"{video_id}.en.vtt"
    if not os.path.exists(vtt_file):
        return None

    with open(vtt_file, "r", encoding="utf-8", errors="ignore") as f:
        raw_vtt = f.read()

    os.remove(vtt_file)

    clean_text = clean_vtt_text(raw_vtt)
    return clean_text if clean_text.strip() else None

# ============================
# MAIN
# ============================

def main():
    youtube = build("youtube", "v3", developerKey=YOUTUBE_API_KEY)

    rows = []
    seen_video_ids = set()

    for query in SEARCH_QUERIES:
        print(f"\n=== QUERY: {query} ===")

        next_page_token = None
        checked = 0

        while checked < MAX_VIDEOS_PER_QUERY:
            req = youtube.search().list(
                q=query,
                part="snippet",
                type="video",
                maxResults=50,
                pageToken=next_page_token
            )
            res = req.execute()

            for item in res["items"]:
                if checked >= MAX_VIDEOS_PER_QUERY:
                    break

                checked += 1
                video_id = item["id"]["videoId"]

                if video_id in seen_video_ids:
                    continue
                seen_video_ids.add(video_id)

                snippet = item["snippet"]
                title = snippet["title"]
                channel = snippet["channelTitle"]
                published = snippet["publishedAt"][:10]
                description = snippet.get("description", "")
                url = f"https://www.youtube.com/watch?v={video_id}"

                if not within_window(snippet["publishedAt"]):
                    continue

                print(f"\nChecking: {title}")
                print(f"  Channel: {channel}")
                print(f"  Date: {published}")

                transcript = get_auto_transcript(video_id)

                topics = assign_topics(transcript)
                if not topics:
                    topics = assign_topics(description)

                if not topics:
                    print("    ❌ No topic match")
                    continue

                print(f"    ✅ Topics: {topics}")

                for topic in topics:
                    rows.append({
                        "Video Title": title,
                        "Channel": channel,
                        "Publish Date": published,
                        "URL": url,
                        "Topic": topic,
                        "Short Quote": (transcript or description)[:90],
                        "Transcript Context": transcript or description
                    })

            next_page_token = res.get("nextPageToken")
            if not next_page_token:
                break

    df = pd.DataFrame(rows).drop_duplicates()
    df.to_excel(OUTPUT_PATH, index=False)

    print("\n==============================")
    print(f"🔥 DONE — Wrote {len(df)} rows to {OUTPUT_PATH}")
    print("==============================")

if __name__ == "__main__":
    main()
