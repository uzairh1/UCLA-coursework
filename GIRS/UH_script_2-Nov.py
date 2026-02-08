import os
from dotenv import load_dotenv 
import tweepy
import pandas as pd

# Assuming your X bearer token is in a .env file that looks like:
#                   X_BEARER_TOKEN=yours_here


# This part just sets up the API access.
load_dotenv()
BEARER_TOKEN = os.getenv("X_BEARER_TOKEN") 

client = tweepy.Client(bearer_token=BEARER_TOKEN) # Tweepy allows us to parse many fields of a given Tweet using our own API access

query = query = '("Eaton Fire" OR "Palisades Fire" OR "Angeles National Forest" OR wildfire) (from:CalFire OR from:NWSLosAngeles OR from:LAFD OR from:LAPDHQ OR from:latimes OR from:NBCLA) -is:retweet lang:en'


# Here, I am searching for "eaton fire".
# There are a bunch more permutations of queries that are possible:
# https://docs.x.com/x-api/posts/search/integrate/build-a-query

# This link could also be of some interest...just looking at the weather service in LA/CA instead of random people's tweets.
# https://docs.x.com/x-api/posts/search/integrate/build-a-query#tracking-a-natural-disaster

# Another one we could try (untested) is something like this, which tracks Tweets from official authorities:

#("Eaton Fire" OR "Palisades Fire" OR "Angeles National Forest" OR wildfire)
#(from:CalFire OR from:NWSLosAngeles OR from:LAFD OR from:LAPDHQ OR from:latimes OR from:NBCLA)
#-is:retweet lang:en -- retweets can be turned off if just interested in detection (so we get the first notice). We can run this with them enabled for preparation/evacuation time.


response = client.search_recent_tweets(
    query = query,
    max_results = 10, # sets maximum number of results to 10, can be changed
    tweet_fields=["created_at", "geo", "context_annotations"] # different types of fields afforded to us by Tweepy
    )

# created_at: time Tweeted (Universal Coordinated Time)
# geo: any sort of geodata (like location) associated with the Tweet
# context_annotations: rudimentary analysis X does about the Tweet (I don't really know what this entails)

# Create pandas dataframe
rows = []
for tweet in response.data:
    rows.append({ # using Tweepy data to make a dataframe
        "Text": tweet.text, 
        "Created at": tweet.created_at,
        "Location": tweet.geo,
        "Context inferred by X about tweet": tweet.context_annotations,
        })

# Saves pandas dataframe to Excel

df = pd.DataFrame(rows)
df.to_csv("eaton_fire_tweets_v2_2-Nov.csv", index=False)
print("Saved!")


# Below is an add-on piece to the script above...
# Basically what it does is it sorts Tweets matching the query per minute and creates a line graph of how many tweets / minute match the query.
# You can uncomment it and it should run with the rest of the above.

'''

import matplotlib.pyplot as plt

df["Created at"] = pd.to_datetime(df["Created at"])
df = df.sort_values("Created at")
df['minute'] = df["Created at"].dt.floor("T")
counts = df.groupby("minute").size()

plt.title("Tweets over Time: 'Eaton fire'")
plt.xlabel("Time (minutes)")
plt.ylabel("Number of Tweets")
plt.grid(True)
plt.tight_layout()
plt.show()

''' 






