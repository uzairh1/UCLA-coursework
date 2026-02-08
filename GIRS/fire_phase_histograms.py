import pandas as pd
import matplotlib.pyplot as plt

# Load the video-level dataset
df = pd.read_excel("video_level_unique_videos_categorized.xlsx")

# Explode topics into one row per phase
df["Phase"] = df["Topics Covered"].str.split(", ")
df_long = df.explode("Phase")

fires = ["Eaton Fire", "Palisades Fire"]
phases = ["Detection", "Notification", "Preparation", "Evacuation"]

for fire in fires:
    for phase in phases:
        count = len(
            df_long[
                (df_long["Fire Category"] == fire) &
                (df_long["Phase"] == phase)
            ]
        )

        plt.figure(figsize=(4, 4))
        plt.bar([phase], [count])

        # Titles and labels
        plt.title(f"{fire}\n{phase} Phase", fontsize=12)
        plt.ylabel("Number of Videos", fontsize=10)

        # Clean x-axis (phase already in title)
        plt.xticks([])

        # Make layout clean
        plt.tight_layout()

        # Save figure
        filename = f"{fire.replace(' ', '_')}_{phase}.png"
        plt.savefig(filename, dpi=300)
        plt.close()
