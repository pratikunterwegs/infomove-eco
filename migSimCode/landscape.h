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
const int landsize = 200;
const int popsize = 3000;
const int tMax = 150; // must always be less than landsize

// green wave params
const int initpeak = 20;
const float peakvalue = 100.f;
const float steepness = 0.01f;

// init landscape struct
class site {
public:
	float resource;
	int nAgents;
	int nAgentsMigrating;
	int totalComp;
	float propAgentsMigrating;

	// func to get agents moving on
	void getMigProp();
};

// init vector of sites as landscape
std::vector<site> landscape(landsize);

//