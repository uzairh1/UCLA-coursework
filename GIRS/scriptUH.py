import os
import json
import requests
from pathlib import Path
from urllib.parse import urlencode

# ==========================
# CONFIGURATION
# ==========================

# 1) Put your Bearer Token in an environment variable:
#    macOS/Linux: export X_BEARER_TOKEN="YOUR_TOKEN_HERE"
#    Windows (PowerShell): $env:X_BEARER_TOKEN="YOUR_TOKEN_HERE"
BEARER_TOKEN = os.getenv("X_BEARER_TOKEN", "AAAAAAAAAAAAAAAAAAAAALLg5QEAAAAA4GAs410f9VQ4KkUySjzETCd7XPk%3DEtgi5LACRWlC9ibqXQB85uyXEUWRD7oKZ7YrfwyN7jbima4Ecd")

# 2) Define your search query.
# You can customize this with additional logical operators (OR, -, from:, etc.)
QUERY = '("Eaton Fire" OR "EatonFire") (wildfire OR fire) lang:en -is:retweet'

# 3) Max number of tweets to fetch per request (10–100)
MAX_RESULTS = 100

# Optionally set time window in ISO-8601 (UTC). If you omit, X uses a default window.
# EXAMPLE format: "2025-01-11T00:00:00Z"
START_TIME =  "2025-01-07T00:00:00Z"  # e.g. "2025-01-11T00:00:00Z"
END_TIME = "2025-01-31T23:59:59Z"    # e.g. "2025-01-20T00:00:00Z"

# ==========================
# HELPER FUNCTIONS
# ==========================

def get_headers():
    if not BEARER_TOKEN:
        raise RuntimeError(
            "Bearer token not set. Set X_BEARER_TOKEN environment variable first."
        )
    return {
        "Authorization": f"Bearer {BEARER_TOKEN}",
        "User-Agent": "EatonFireSearchScript/1.0"
    }


def build_url(query, max_results=10, start_time=None, end_time=None, next_token=None):
    # Depending on your access, this may also work:
    # base_url = "https://api.twitter.com/2/tweets/search/recent"
    base_url = "https://api.x.com/2/tweets/search/recent"

    params = {
        "query": query,
        "max_results": max_results,
        # Add fields you care about:
        "tweet.fields": "created_at,author_id,public_metrics,conversation_id",
        "expansions": "author_id",
        "user.fields": "username,name"
    }

    if start_time:
        params["start_time"] = start_time
    if end_time:
        params["end_time"] = end_time
    if next_token:
        params["next_token"] = next_token

    url = f"{base_url}?{urlencode(params)}"
    return url


def search_eaton_fire(query=QUERY, max_results=MAX_RESULTS, start_time=START_TIME,
                      end_time=END_TIME, pages=1):
    """
    Search X for tweets matching query.
    :param query: X search query string
    :param max_results: tweets per page (10–100)
    :param start_time: ISO 8601 start time or None
    :param end_time: ISO 8601 end time or None
    :param pages: how many pages of results to fetch
    :return: list of tweet objects (as dicts)
    """
    headers = get_headers()
    next_token = None
    all_tweets = []

    for page in range(pages):
        url = build_url(
            query=query,
            max_results=max_results,
            start_time=start_time,
            end_time=end_time,
            next_token=next_token,
        )
        resp = requests.get(url, headers=headers)
        if resp.status_code != 200:
            print(f"Request failed with status {resp.status_code}: {resp.text}")
            break

        data = resp.json()
        tweets = data.get("data", [])
        users = {u["id"]: u for u in data.get("includes", {}).get("users", [])}

        if not tweets:
            print("No more tweets found.")
            break

        for t in tweets:
            user = users.get(t["author_id"], {})
            tweet_url = f"https://x.com/{user.get('username', 'unknown')}/status/{t['id']}"
            print("------------------------------------------------------------")
            print(f"Time:   {t['created_at']}")
            print(f"User:   {user.get('name', '')} (@{user.get('username', '')})")
            print(f"URL:    {tweet_url}")
            print(f"Text:   {t['text']}")
            metrics = t.get("public_metrics", {})
            print(
                f"Engagement: ♥ {metrics.get('like_count',0)}  "
                f"↩ {metrics.get('reply_count',0)}  "
                f"🔁 {metrics.get('retweet_count',0)}"
            )
            all_tweets.append({
                **t,
                "username": user.get("username"),
                "name": user.get("name"),
                "tweet_url": tweet_url,
            })

        next_token = data.get("meta", {}).get("next_token")
        if not next_token:
            print("No next_token, end of results.")
            break

    return all_tweets


if __name__ == "__main__":
    # Example: fetch up to 2 pages of 20 tweets each
    results = search_eaton_fire(pages=2)
    print(f"\nTotal tweets fetched: {len(results)}")

    # ==== Save to Desktop as JSON ====
    desktop = Path.home() / "Desktop"
    desktop.mkdir(parents=True, exist_ok=True)  # just in case

    output_file = desktop / "eaton_fire_results.json"

    with open(output_file, "w", encoding="utf-8") as f:
        json.dump(results, f, indent=2, ensure_ascii=False)

    print(f"Saved results to: {output_file}")
