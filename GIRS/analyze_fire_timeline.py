import os
import requests
import datetime
import random
import pandas as pd
import re
from dotenv import load_dotenv
from zoneinfo import ZoneInfo
import time

# ==========================
# CONFIGURATION
# ==========================
load_dotenv()
BEARER_TOKEN = os.getenv("X_BEARER_TOKEN")
if not BEARER_TOKEN:
    raise ValueError("❌ Bearer token not found. Put it in your .env file as X_BEARER_TOKEN=")

# Fire and location keywords
FIRE_KEYWORDS = {
    "Eaton Fire": ['"Eaton Fire"', '"Eaton Canyon"', "Altadena"],
    "Palisades Fire": ['"Palisades Fire"', '"Pacific Palisades"', "Topanga"]
}

# Choose which fire to analyze
FIRE_NAME = "Eaton Fire"  # change to "Palisades Fire" if needed
query_terms = " OR ".join(FIRE_KEYWORDS[FIRE_NAME])
query = f"({query_terms}) (fire OR wildfire OR smoke OR flames OR evacuate OR evacuation) -is:retweet lang:en"

# Random day within past 7 days
today = datetime.datetime.utcnow().date()
days_ago = random.randint(1, 6)
random_day = today - datetime.timedelta(days=days_ago)

print(f"🔥 Fetching tweets for {FIRE_NAME} on {random_day} (UTC)")

# ==========================
# DATA COLLECTION
# ==========================
url = "https://api.twitter.com/2/tweets/search/recent"
headers = {"Authorization": f"Bearer {BEARER_TOKEN}"}
params = {
    "query": query,
    "max_results": 100,
    "tweet.fields": "created_at,public_metrics,lang",
}

all_tweets = []
next_token = None
page_count = 0
MAX_PAGES = 3   # Limit to avoid long scraping sessions

while True:
    if next_token:
        params["next_token"] = next_token

    response = requests.get(url, headers=headers, params=params)
    page_count += 1

    # --- Exit immediately if rate limit hit ---
    if response.status_code == 429:
        print("⚠️  Rate limit hit — exiting early to avoid long wait. Try again later.")
        break

    elif response.status_code != 200:
        raise Exception(f"Request failed: {response.status_code}, {response.text}")

    # --- Log remaining requests (if available) ---
    remaining = response.headers.get("x-rate-limit-remaining")
    limit = response.headers.get("x-rate-limit-limit")
    if remaining and limit:
        print(f"📦 Request {page_count}: Remaining {remaining}/{limit}")

    data = response.json()
    tweets = data.get("data", [])
    if not tweets:
        break

    all_tweets.extend(tweets)
    meta = data.get("meta", {})
    next_token = meta.get("next_token")

    if page_count >= MAX_PAGES:
        print(f"🛑 Stopping early after {MAX_PAGES} pages to avoid hitting rate limits.")
        break

    if not next_token:
        break

    time.sleep(10)  # short polite delay between pages

print(f"✅ Collected {len(all_tweets)} tweets (across {page_count} request(s))")

# ==========================
# DATA CLEANING / CATEGORIZATION
# ==========================
def clean_text(txt):
    txt = re.sub(r"http\S+", "", txt)
    txt = re.sub(r"[@#]\S+", "", txt)
    return txt.strip()

def categorize_phase(text):
    text_l = text.lower()
    if any(x in text_l for x in ["evacuation", "evacuate", "shelter", "leave now"]):
        return "evacuation"
    elif any(x in text_l for x in ["prepare", "ready", "warning", "alert"]):
        return "preparation"
    elif any(x in text_l for x in ["fire", "smoke", "burning", "visible flames"]):
        return "detection"
    else:
        return "other"

local_zone = ZoneInfo("America/Los_Angeles")

df = pd.DataFrame([
    {
        "time_utc": t["created_at"],
        "text": clean_text(t["text"]),
        "likes": t["public_metrics"]["like_count"],
        "retweets": t["public_metrics"]["retweet_count"],
    }
    for t in all_tweets
])

if df.empty:
    print("⚠️  No tweets collected. Try again later or adjust your query.")
    exit()

df["time_utc"] = pd.to_datetime(df["time_utc"], utc=True)
df["time_local"] = df["time_utc"].dt.tz_convert(local_zone)
df["phase"] = df["text"].apply(categorize_phase)
df["engagement"] = df["likes"] + df["retweets"]

# ==========================
# TIMELINE ANALYSIS
# ==========================
def phase_time(phase):
    if phase in df["phase"].values:
        return df[df["phase"] == phase]["time_local"].min()
    return None

detection_t = phase_time("detection")
prep_t = phase_time("preparation")
evac_t = phase_time("evacuation")

print("\n🕒 Estimated Timeline:")
if detection_t: print(f"  🔎 Detection phase: {detection_t}")
if prep_t: print(f"  ⚠️  Preparation phase: {prep_t}  (Δ from detection: {prep_t - detection_t if detection_t and prep_t else '—'})")
if evac_t: print(f"  🚪 Evacuation phase: {evac_t}  (Δ from preparation: {evac_t - prep_t if evac_t and prep_t else '—'})")

# ==========================
# OUTPUT
# ==========================
out_name = f"{FIRE_NAME.lower().replace(' ', '_')}_{random_day}_timeline.csv"
df.to_csv(out_name, index=False)
print(f"\n✅ Saved structured data to {out_name}")
print("Columns:", list(df.columns))
