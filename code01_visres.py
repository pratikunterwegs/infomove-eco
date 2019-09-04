# this is some prelim python code

# check python path
import sys

# should yield python 3.7 file path
for p in sys.path:
    print(p)

# load libraries
import pandas as pd # similar to dplyr! yay!
import os  # has list dir functions etc
import numpy as np  # some matrix functions
import scipy.special

import seaborn as sns
import matplotlib.pyplot as plt
from scipy import stats

# check the current working directory
os.getcwd()
currentWd = p # os.path.dirname(os.path.abspath(__file__)) #os.getcwd()

# check again
print(currentWd)

# read in data
d = pd.read_csv("migSimCode/dataOut.csv")
for col in d.columns:
    print(col)

#### summarised data frame
# get data summary of mean dist peak
dsmrpeakdist = d.assign(peakdist = d.peakpos - d.pos)\
    .groupby(['gen','id','movep'])['peakdist'] \
    .agg(['mean', 'var'])

# get summary of leader switches per id and gen
dsmrleadswitch = d.groupby(['gen', 'id'])\
    .apply(lambda x: (len(x.leader.unique())))

#### this is really bad, seems like the agents fix on their first
# neighbour and are not really choosing who to follow


# summarise as in R for historgram of distance to peak over time
g = sns.FacetGrid(col="gen", row="time", margin_titles=True, data=d)
bins = np.linspace(-100, 900, 100)
g.map(plt.hist, "pos", color="red", bins=bins, density=1, alpha=0.8)
g.map(plt.hist, x=" peakpos", ymin=0,ymax=10, color="steelblue", alpha=0.8)