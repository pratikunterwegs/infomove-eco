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

# summarise as in R for historgram of distance to peak over time
g = sns.FacetGrid(col="gen", margin_titles=True, data=d, col_wrap=5)
# bins = np.linspace(0, 700, 100)
g.map(plt.hist, " pos", color="red")