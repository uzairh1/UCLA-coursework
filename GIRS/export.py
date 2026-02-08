import json
from pathlib import Path
import pandas as pd

# ----------------------------
# RUN DIRECTORY
# ----------------------------

RUN_DIR = Path("output/v1.5")

# ----------------------------
# PATHS
# ----------------------------

INPUT_JSONL = RUN_DIR / "timeline_hits_v1.5.jsonl"
OUTPUT_XLSX = RUN_DIR / "timeline_hits_by_fire.xlsx"

# ----------------------------
# SAFETY CHECK
# ----------------------------

if not INPUT_JSONL.exists():
    raise FileNotFoundError(f"Input file not found:\n{INPUT_JSONL.resolve()}")


# ----------------------------
# HELPERS
# ----------------------------

def safe_sheet_name(name: str) -> str:
    return (
        name.replace("/", "_")
            .replace("\\", "_")
            .replace("?", "")
            .replace("*", "")
            .replace("[", "")
            .replace("]", "")
    )[:31]


# ----------------------------
# LOAD JSONL
# ----------------------------

rows = []
with INPUT_JSONL.open("r", encoding="utf-8") as f:
    for line in f:
        if line.strip():
            rows.append(json.loads(line))

df = pd.DataFrame(rows)

# ----------------------------
# SORTING
# ----------------------------

EVENT_TYPE_PRIORITY = {
    "explicit_time": 0,
    "relative_time": 1,
    "none": 2,
}

df["event_type_rank"] = df["event_time_type"].map(EVENT_TYPE_PRIORITY).fillna(3)

df["sort_time"] = df["event_time"].where(
    df["event_time"].notna() & (df["event_time"] != ""),
    df["published_at"]
)

df["sort_time"] = pd.to_datetime(df["sort_time"], errors="coerce")

df = df.sort_values(["fire_name", "event_type_rank", "sort_time"])

# ----------------------------
# COLUMN ORDER
# ----------------------------

preferred_order = [
    "fire_name",
    "stage",
    "event_time",
    "event_time_type",
    "published_at",
    "location_text",
    "video_url",
    "comment_url",
    "video_comment_count",
    "text",
]

df = df[preferred_order]

df_event_only = df[df["event_time_type"].isin(["explicit_time", "relative_time"])]

# ----------------------------
# EXPORT
# ----------------------------

with pd.ExcelWriter(OUTPUT_XLSX, engine="openpyxl") as writer:
    df.to_excel(writer, sheet_name="ALL_FULL", index=False)
    df_event_only.to_excel(writer, sheet_name="ALL_EVENT_ONLY", index=False)

    for fire, fire_df in df.groupby("fire_name"):
        fire_df.to_excel(
            writer,
            sheet_name=safe_sheet_name(fire),
            index=False
        )

print("Export complete.")
print(f"Output: {OUTPUT_XLSX.resolve()}")
print(f"Rows: {len(df)}")
print(f"Fires: {df['fire_name'].unique().tolist()}")
