# this is some prelim python code

# check python path
import sys

# load libraries
import pandas as pd # similar to dplyr! yay!
import os  # has list dir functions etc

import seaborn as sns
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# should yield python 3.7 file path
for p in sys.path:
    print(p)

# check the current working directory
os.getcwd()
currentWd = p # os.path.dirname(os.path.abspath(__file__)) #os.getcwd()

# check again
print(currentWd)

# read in data
data = pd.read_csv("migSimCode/data_agent_summary.csv")
for col in data.columns:
    print(col)

# plot agent values
sns.scatterplot(x='gen', y='prop_indep', data=data, linewidth=0, s=1)
sns.scatterplot(x='gen', y='tradeoff', data=data, linewidth=0, s=1)

# 3d plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(xs = data['prop_indep'], ys= data['tradeoff'], zs= data['energy'], c=data['gen'], cmap="Reds")