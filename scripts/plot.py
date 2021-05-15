import seaborn as sns
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

map = "fr78"
window_size = 10
df = (pd
    .read_csv(f"out/{map}.csv").sort_values("Distance")
    #.rolling(window_size)
    #.mean()
    .melt("Distance")
)
split = df["variable"].str.split(expand=True)
del df["variable"]
df["Algorithm"] = split[0]
df["Variable"] = split[1]
df = df.replace("Time", "Time (ms)")

g = sns.FacetGrid(df, col="Variable", hue="Algorithm")
g.map(sns.scatterplot, "Distance", "value")
plt.gca().set(yscale="log")
plt.savefig(f"out/{map}.pdf")