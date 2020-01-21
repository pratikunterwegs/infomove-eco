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
const int popsize = 1000; // not too many

// agent params
const float moveDist = 0.01f;

// landscape params
const int landPoints = 100;
const float maxFood = 10.f;
const float maxDepletion = 0.01f;
const float deadZone = 0.001f * 1.0f; // 0.1 % of the landscape size is a deadzone around each agent
const float deplZone = 0.005f * 1.0f; // 0.5 % of the landscape size on each side of the agent is a deadzone

// time params
const int genmax = 1000; //arbit choice
const int tMax = 25;

// rand number
std::random_device rd;
std::mt19937_64 rng;
//std::normal_distribution<float> normDist(1.f, 6.f);

// end here
