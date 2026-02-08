import os
import requests
import datetime
import random
import pandas as pd
from dotenv import load_dotenv

# --- 1. Load your token from .env ---
load_dotenv()  # loads the .env file automatically
BEARER_TOKEN = os.getenv("X_BEARER_TOKEN")

if not BEARER_TOKEN:
    print("DEBUG token:", repr(BEARER_TOKEN))
    raise ValueError("❌ Bearer token still not found!")

# --- 2. Pick a random day in the past week ---
today = datetime.datetime.utcnow().date()
days_ago = random.randint(1, 6)
random_day = today - datetime.timedelta(days=days_ago)

start_time = datetime.datetime.combine(random_day, datetime.time.min).isoformat() + "Z"
end_time = datetime.datetime.combine(random_day, datetime.time.max).isoformat() + "Z"

print(f"🔍 Searching tweets from {random_day}...")

# --- 3. Set up your query and headers ---
query = "wildfire OR fire -is:retweet lang:en"  # sample topic
url = "https://api.twitter.com/2/tweets/search/recent"
headers = {"Authorization": f"Bearer {BEARER_TOKEN}"}
params = {
    "query": query,
    "max_results": 20,  # small sample
    "tweet.fields": "created_at,public_metrics,author_id",
}

# --- 4. Send request to X API ---
response = requests.get(url, headers=headers, params=params)
if response.status_code != 200:
    raise Exception(f"Request failed: {response.status_code}, {response.text}")

data = response.json()

# --- 5. Convert to a simple table (DataFrame) ---
tweets = data.get("data", [])
if not tweets:
    print("No tweets found.")
else:
    df = pd.DataFrame([
        {
            "time": t["created_at"],
            "text": t["text"][:100] + "...",
            "likes": t["public_metrics"]["like_count"],
            "retweets": t["public_metrics"]["retweet_count"]
        }
        for t in tweets
    ])
    print(df)
    df.to_csv("tweets_sample.csv", index=False)
    print("✅ Saved to tweets_sample.csv")
