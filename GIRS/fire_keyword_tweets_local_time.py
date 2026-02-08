import os
import requests
import datetime
import random
import pandas as pd
from dotenv import load_dotenv
from zoneinfo import ZoneInfo  # built-in since Python 3.9

# --- 1. Load credentials ---
load_dotenv()
BEARER_TOKEN = os.getenv("X_BEARER_TOKEN")
if not BEARER_TOKEN:
    raise ValueError("❌ Bearer token not found! Make sure it's in your .env file.")

# --- 2. Pick your keyword here ---
KEYWORD = "Eaton Fire"  # ← change this to "Palisades Fire" if you want

# --- 3. Pick a random day in the past week ---
today = datetime.datetime.utcnow().date()
days_ago = random.randint(1, 6)
random_day = today - datetime.timedelta(days=days_ago)

start_time = datetime.datetime.combine(random_day, datetime.time.min).isoformat() + "Z"
end_time = datetime.datetime.combine(random_day, datetime.time.max).isoformat() + "Z"

print(f"🔥 Searching tweets for '{KEYWORD}' on {random_day} (UTC)...")

# --- 4. Set up API query ---
query = f'"{KEYWORD}" -is:retweet lang:en'
url = "https://api.twitter.com/2/tweets/search/recent"
headers = {"Authorization": f"Bearer {BEARER_TOKEN}"}
params = {
    "query": query,
    "max_results": 100,  # small sample
    "tweet.fields": "created_at,public_metrics,author_id",
}

# --- 5. Request data from X API ---
response = requests.get(url, headers=headers, params=params)
if response.status_code != 200:
    raise Exception(f"Request failed: {response.status_code}, {response.text}")

data = response.json()
tweets = data.get("data", [])

# --- 6. Convert to DataFrame ---
if not tweets:
    print("⚠️ No tweets found for that keyword and date.")
else:
    df = pd.DataFrame([
        {
            "time_utc": t["created_at"],
            "text": t["text"][:100] + "...",
            "likes": t["public_metrics"]["like_count"],
            "retweets": t["public_metrics"]["retweet_count"]
        }
        for t in tweets
    ])

    # --- 7. Convert to local time (adjust this zone) ---
    local_zone = ZoneInfo("America/Los_Angeles")  # change as needed
    df["time_local"] = pd.to_datetime(df["time_utc"]).dt.tz_convert(local_zone)

    # --- 8. Show and save ---
    print(df[["time_local", "text", "likes", "retweets"]])
    out_name = f"tweets_{KEYWORD.lower().replace(' ', '_')}.csv"
    df.to_csv(out_name, index=False)
    print(f"✅ Saved {len(df)} tweets to {out_name}")
