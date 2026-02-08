import re

INPUT = "youtube_full.txt"
OUTPUT = "events_clean.txt"

# --- patterns ---

TIME = re.compile(
    r'([0-9]{1,2}:[0-9]{2}\s?(am|pm)|'
    r'[0-9]{1,2}\s?(am|pm)|'
    r'\d+\s+minutes later|'
    r'\d+\s+minute later|'
    r'\d+\s+hours later|'
    r'\d+\s+hour later|'
    r'overnight)',
    re.I
)

DECISION = re.compile(
    r'\b(evacuated|left|got out|ran out|packed|decided to leave|decided to go|fled|returned|noticed the fire|saw the fire)\b',
    re.I
)

PRONOUN = re.compile(r'\b(i|we|my|our|they)\b', re.I)

# --- read & clean ---

with open(INPUT, "r", encoding="utf-8", errors="ignore") as f:
    text = f.read()

text = re.sub(r'https?://\S+', '', text)
text = re.sub(r'\bSpeaker\s+\d+\b', '', text)
text = re.sub(r'\s+', ' ', text)

# --- split into rough sentences ---
sentences = re.split(r'[.!?]', text)

events = set()

for s in sentences:
    if not TIME.search(s):
        continue
    if not DECISION.search(s):
        continue

    time = TIME.search(s).group(0)
    decision = DECISION.search(s).group(0)

    who = "we"
    p = PRONOUN.search(s)
    if p:
        who = p.group(0).lower()

    event = f"{time} — {who} {decision}"
    events.add(event.strip())

# --- write ---
with open(OUTPUT, "w", encoding="utf-8") as f:
    for e in sorted(events):
        f.write(e + "\n")

print(f"Wrote {len(events)} clean events to {OUTPUT}")
