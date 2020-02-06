#!/bin/bash
# activate the virtual env and install packages
pip3 install virtualenv
mkdir venv && cd venv

# using python 3
python3 -m venv venv

cd ..

source "venv/bin/activate"

# check which python
which python

# get pandas and numpy
pip3 install pandas
pip3 install numpy

# source the python script
python codeSimData.py
