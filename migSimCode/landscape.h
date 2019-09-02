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

// size parameters
// const int landsize = 1000; // not necessary
const int popsize = 500; // not too many
const int tMax = 100; // not too high

// green wave params
const float initpeak = 10.f; // arbitrary choice greater than 0
float currentpeak = initpeak; // to begin
// const int finalpeak = landsize - initpeak + 100;

// wave velocity is a float drawn from a normal dist
// normal dist
std::mt19937_64 rng;
std::normal_distribution<float> normDist(1.f, 6.f);

const float waveVelocity = normDist(rng); // how fast the peak moves
const float peakvalue = 100.f;
const float steepness = 0.05f;

// seasons or replicates params
const int genmax = 25; //arbit choice

// end here