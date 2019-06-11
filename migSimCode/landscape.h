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
const int landsize = 20;
const int popsize = 3000;
const int tMax = 10; // must always be less than landsize

// green wave params
const int initpeak = 2;
const float peakvalue = 100.f;
const float steepness = 0.01f;

// init landscape
std::vector<float> landscape(landsize);
