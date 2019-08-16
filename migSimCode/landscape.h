#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <cassert>
#include <random>
#include <chrono>
#include <string>

// size parameters
// const int landsize = 1000; // not necessary
const int popsize = 1000; // not too many
const int tMax = 100; // around 200 days

// green wave params
const float initpeak = 10.f; // arbitrary choice greater than 0
float currentpeak = initpeak; // to begin
// const int finalpeak = landsize - initpeak + 100;

// wave velocity is a float drawn from a normal dist
// normal dist
std::mt19937_64 rng;
std::normal_distribution<float> normDist(1.f, 1.f);

const float waveVelocity = normDist(rng); // how fast the peak moves
const float peakvalue = 100.f;
const float steepness = 0.05f;

// seasons or replicates params
const int nGen = 10000; //arbit choice

// end here