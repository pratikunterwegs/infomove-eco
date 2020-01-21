# this is some prelim python code

# check python path
import sys

# load libraries
import pandas as pd # similar to dplyr! yay!
import os  # has list dir functions etc
import numpy as np  # some matrix functions
import scipy.special

import seaborn as sns
import matplotlib.pyplot as plt
from scipy import stats

# should yield python 3.7 file path
for p in sys.path:
    print(p)

# check the current working directory
os.getcwd()
currentWd = p # os.path.dirname(os.path.abspath(__file__)) #os.getcwd()

# check again
print(currentWd)

# read in data
dataAgents = pd.read_csv("migSimCode/dataAgents.csv")
for col in dataAgents.columns:
    print(col)

# filter generations modulo 50
dataAgents = dataAgents[(dataAgents['gen'] % 50 == 0) &
                        (dataAgents['time'] % 10 == 0)]

# summarise agents
dataAgents = dataAgents.assign(indep = (dataAgents.leader == dataAgents.id))
dataAgents = dataAgents.groupby(['gen', 'eeParam'])['indep']\
    .sum()
dataAgents = dataAgents.reset_index()

# plot landscape values
h = sns.FacetGrid(dataAgents, col="gen")
h.map(plt.scatter, "eeParam", "indep", s=0.2)
