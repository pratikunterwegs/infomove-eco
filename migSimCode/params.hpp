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

/// function to print vector output
//template <class T>
//void out_vec(const std::vector<T>& vec)
//{
//	std::copy(std::begin(vec), std::end(vec), std::ostream_iterator<T>(std::cout, " "));
//	std::cout << "\n";
//}

// population params
const int popsize = 100; // not too many

// landscape params
const int n_patches = 100;
const float maxFood = 1.f;
const float maxDepletion = 0.1f;
const float max_regrowth = 0.2f;
const float predation_cost = 0.1f;

// time params
const int genmax = 10000; //arbit choice
const int tMax = 100;
const int foraging_turns = 10;

// agent params
const float move_cost = 0.01f;

// random number generators
std::random_device rd;
std::mt19937_64 rng;

// end here
