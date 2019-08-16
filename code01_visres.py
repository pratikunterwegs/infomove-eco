# this is some prelim python code

# check python path
import sys

# should yield python 3.7 file path
for p in sys.path:
    print(p)

import pandas as pd # similar to dplyr! yay!
import os  # has list dir functions etc
import numpy as np  # some matrix functions
from scipy import misc
import matplotlib.pyplot as plt

# check the current working directory
os.getcwd()
currentWd = p # os.path.dirname(os.path.abspath(__file__)) #os.getcwd()

# check again
print(currentWd)

# read in data
d = pd.read_csv("migSimCode/agentpos.csv")