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
const int popsize = 500; // not too many
const int tMax = 10; // not too high

//// green wave params
//float initpeak = 100.f; // arbitrary choice greater than 0
//float currentpeak = initpeak; // to begin
//float altpeak = 50.f;
//// const int finalpeak = landsize - initpeak + 100;
//
//// wave velocity is a float drawn from a normal dist
//// normal dist
//std::mt19937_64 rng;
//std::normal_distribution<float> normDist(1.f, 6.f);

// seasons or replicates params
const int genmax = 200; //arbit choice

// end here