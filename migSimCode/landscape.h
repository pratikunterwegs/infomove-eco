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
const int landsize = 200; // keep even
const int popsize = 30; // not too many
const int tMax = 150; // must always be less than landsize

// green wave params
const int initpeak = 0; // arbitrary choice
int currentpeak = initpeak; // to begin
const int finalpeak = landsize - initpeak;
const int waveVelocity = 20; // how fast the peak moves
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

	// func to get agents moving on
	void updateLandscape();
};

// init vector of sites as landscape
std::vector<site> landscape(landsize);

// update func
void site::updateLandscape()
{
	// maybe not here
}

//