#pragma once

/// header controlling depletion dynamics
// to be imported after agents.h

#include "landscape.h"
#include "agents.h"


// some params
float regrowth = 0.1f;

// make gridcell class
class gridcell
{
public:
	gridcell() : nAgents(0), nTotAgents(0), dFood(1.0), dRegrowth(1.0) {};
	~gridcell() {};

	// each gridcell stores nAgents and food
	int nAgents, nTotAgents; float dFood, dRegrowth;

};

// init landscape of length maxland 1000
std::vector<gridcell> landscape(maxLand);

/// functions of landscape size
// func to extend landscape
//void extendLandscape()
//{
//	float movemax = 1.f;
//	// get max intrinsic move param of population
//	for (int p = 0; p < popsize; p++) 
//	{
//		movemax = (movemax > population[p].moveDist) ? movemax : population[p].moveDist;
//	}
//
//	// how much to extend
//	int landExtend = static_cast<int>(floor(movemax)) >= landscape.size();
//
//	// extend landscape up to the max land size
//	if (landscape.size() + landExtend <= 1000) 
//	{
//		// compare movemax with landscape length and expand
//		if (static_cast<int>(floor(movemax)) >= landscape.size())
//		{
//			// print extension
//			// make vector of extra grid cells
//			std::vector<gridcell> extraland(static_cast<int>(floor(movemax)) - landscape.size() + 1); // adding one to handle errors
//
//			landscape.insert(landscape.end(), extraland.begin(), extraland.end());
//			// print success extend
//			std::cout << "land extended to " << landscape.size() << "\n";
//		}
//	}
//}

/// functions of landscape dynamics
// function to generate food
void makeFoodOnLand()
{
	for (int l = 0; l < landscape.size(); l++)
	{
		landscape[l].dFood += regrowth;
	}
}

// function to count agents on cell
void addAgentsToLand()
{
	for (int p = 0; p < popsize; p++)
	{
		// get agent integer position
		int agentPos = static_cast<int> (floor(population[p].moveDistCopy));

		// print agent pos
		// std::cout << "agent pos = " << agentPos << "\n";

		// if agents try to cross max land they stop at max land
		int whichLand = (agentPos >= maxLand) ? (maxLand - 1) : agentPos;
		
		// add agent to relevant grid cell
		landscape[whichLand].nAgents += 1;
	}
}

// move function to get intake here
/// function to get energy
void doGetFood(const int& whichAgent)
{
	// get agent position
	int agentPos = static_cast<int> (floor(population[whichAgent].moveDistCopy));

	// if agents try to cross max land they stop at max land
	int whichLand = (agentPos >= maxLand) ? (maxLand - 1) : agentPos;

	// count agents at pos
	int neighbours = landscape[whichLand].nAgents > 0 ? landscape[whichLand].nAgents : 1;

	// value of food shared with neighbours
	float food = static_cast<float> (landscape[whichLand].dFood) / (static_cast<float>(neighbours));

	// energy in if move is true - loop following is penalised
	agentEnergyVec[whichAgent] += (population[whichAgent].move) ? food : 0.000001;
}

/// function to deplete landscape
// all food is depleted if agents visit
void depleteLand()
{
	for (int l = 0; l < landscape.size(); l++)
	{
		landscape[l].dFood = (landscape[l].nAgents > 0) ? 0.f : landscape[l].dFood;
	}
}

/// func to reset landscape
void resetAgentsOnLand()
{
	for (int l = 0; l < landscape.size(); l++)
	{
		landscape[l].nAgents = 0;
	}
}

// ends here
