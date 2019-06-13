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
const int landsize = 1000; // keep even
const int popsize = 100; // not too many
const int tMax = 200; // must always be less than landsize

// green wave params
const int initpeak = 10; // arbitrary choice greater than 0
int currentpeak = initpeak; // to begin
const int finalpeak = landsize - initpeak + 100;
const int waveVelocity = 5; // how fast the peak moves
const float peakvalue = 100.f;
const float steepness = 0.01f;

// season params
const int nSeasons = 100; //arbit choice

// init landscape struct
class site {
public:
	float resource;
	int nAgents;
	int nAgentsMigrating;
	int totalComp;
	float propAgentsMigrating;

};

// init vector of sites as landscape
std::vector<site> landscape(landsize);


// flip landscape
void flipLand(std::vector<site> &landscape)
{
	// flip the values
	std::rotate(landscape.begin(), landscape.end() - 1, landscape.end());
	
	// flip peak location
	currentpeak = initpeak;
}

//