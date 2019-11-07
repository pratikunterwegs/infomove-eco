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

// population params
const int popsize = 100; // not too many

// landscape params
const int landPoints = 10;
const float maxLandPos = 20.f;
const float maxFood = 1.f;
const float maxDepletion = 0.01f;
const float depletionSlope = 5.f;
const float depletionRadius = 2.f;

// time params
const int genmax = 1000; //arbit choice
const int tMax = 25;

// rand number
std::mt19937_64 rng;
//std::normal_distribution<float> normDist(1.f, 6.f);

// end here