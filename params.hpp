#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <chrono>
#include <string>
#include <fstream>
#include <ostream>

// gsl libraries
#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>


// population params
const int popsize = 500; // not too many
const int epoch = 1000;

// landscape params
const int n_patches = 5000;
const float maxFood = 1.f;

// mutation params
const float m_shift = 0.01f;
const float m_prob = 0.01f;
const float m_shift_a = 0.05f;

// make a gsl rng and a regular rng
gsl_rng * r ;
std::mt19937 rng;

// end here
