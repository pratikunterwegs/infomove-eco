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
template <class T>
void out_vec(const std::vector<T>& vec)
{
	std::copy(std::begin(vec), std::end(vec), std::ostream_iterator<T>(std::cout, " "));
	std::cout << "\n";
}

// size parameters
// const int landsize = 1000; // not necessary
const int popsize = 100; // not too many
const int tMax = 25; // not too high

// max landsize
const int maxLand = 20;

// rand number
std::mt19937_64 rng;
//std::normal_distribution<float> normDist(1.f, 6.f);

// seasons or replicates params
const int genmax = 1000; //arbit choice

// landscape carrying capacity
const float maxFood = 1.f;

// end here