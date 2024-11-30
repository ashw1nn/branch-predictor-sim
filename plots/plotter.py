import numpy as np
import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt

# Create a column to identify the DataFrame source
df1 = pd.read_csv("plots/bimodal_misprediction_rates_gcc_B.csv")
df2 = pd.read_csv("plots/bimodal_misprediction_rates_jpeg_B.csv")
df3 = pd.read_csv("plots/gshare_misprediction_rates_gcc_B.csv")
df4 = pd.read_csv("plots/gshare_misprediction_rates_jpeg_B.csv")

plt.figure(figsize=(7, 5))

# Plot using seaborn with custom line and marker colors
# sns.lineplot(data=df2, x='M', y='Misprediction Rate',
#              marker='o', color='navy',  # Navy blue line
#              markerfacecolor='orange', markeredgecolor='black', markersize=8,  # Orange markers with black edges
#              linewidth=3, legend='brief')

sns.lineplot(data=df4, x='M', y='Misprediction Rate', hue='N', 
             marker='o', palette='Set2',  # Use Set2 color palette for hue
             markerfacecolor='orange', markeredgecolor='black', markersize=8,  # Orange markers with black edges
             linewidth=3)

# Adding labels and title
plt.xlabel('M (No. of PC bits)')
plt.ylabel('Misprediction Rate')
plt.title('[Gshare JPEG] Misprediction Rate vs M')

# Show legend explicitly
# plt.legend()

plt.savefig('plots/q4_plot_B.pdf')
# Display the plot
plt.show()
