#pragma once

/// header controlling depletion dynamics
// to be imported after agents.h

#include "landscape.h"
#include "agents.h"

// make gridcell class
class gridcell
{
public:
	gridcell() : nAgents(0), dFood(0.0) {};
	~gridcell() {};

	// each gridcell stores nAgents and food
	int nAgents; double dFood;

};

/// functions of landscape size
// func to init landscape
void initLandscape
