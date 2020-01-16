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

# dataLand = pd.read_csv("migSimCode/dataLand.csv")
# for col in dataLand.columns:
#     print(col)

# filter generations modulo 50
dataAgents = dataAgents[(dataAgents['gen'] > 40) &
                        (dataAgents['gen'] < 100) &
                        (dataAgents['time'] % 10 == 0)]

# summarise as in R for histogram of distance to peak over time
g = sns.FacetGrid(col="gen", row="time", margin_titles=True, data=dataAgents)
bins = np.linspace(0, 1, 20)
g.map(plt.hist, "pos", color="steelblue", bins=bins)