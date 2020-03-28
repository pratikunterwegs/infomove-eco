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
const int popsize = 25; // not too many
const int leader_choices = 10;

// landscape params
const int n_patches = 10;
const float maxFood = 1.f;
const float maxDepletion = 0.1f;
const float max_regrowth = 0.2f;
const float predation_cost = 0.1f;

// time params
const int genmax = 10; //arbit choice
const int tMax = 10;
const int foraging_turns = 10;
const int genmax_shift = 10;

// environmental regime
const std::vector<float> vec_RHO = {0.f, 0.5f, 1.f, 1.5f, 2.f, 2.5f, 3.f, 3.5f, 4.f, 4.5f, 5.f};
const std::vector<float> vec_PHI = {};
const int DELTA = 100;

// agent params
const float move_cost = 0.01f;

// random number generators
std::random_device rd;
std::mt19937_64 rng;

// end here
