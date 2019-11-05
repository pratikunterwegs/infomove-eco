#pragma once

/// header controlling depletion dynamics
// to be imported after agents.h

#include "landscape.h"
#include "agents.h"

// make gridcell class
class gridcell
{
public:
	gridcell() : nAgents(0), nTotAgents(0), dFood(1.f) {};
	~gridcell() {};

	// each gridcell stores nAgents and food
	int nAgents, nTotAgents; float dFood, dRegrowth;

};

// init landscape of length maxland 1000
std::vector<gridcell> landscape(maxLand);

// function to count agents on cell
void addAgentsToLand()
{
	for (int p = 0; p < popsize; p++)
	{
		// get agent integer position
		int agentPos = static_cast<int> (floor(population[p].moveAngleCopy));

		// wrap agent position to landscape
		agentPos = (agentPos % maxLand);

		// print agent pos
		// std::cout << "agent pos = " << agentPos << "\n";

		// add agent to relevant grid cell
		landscape[agentPos].nAgents += 1;

		// add total agents
		landscape[agentPos].nTotAgents += 1;
	}
}

// move function to get intake here
/// function to get energy
void doGetFood(const int& whichAgent)
{
	// get agent integer position
	int agentPos = static_cast<int> (floor(population[whichAgent].moveAngleCopy));

	// wrap agent position to landscape
	agentPos = (agentPos % maxLand);

	// count agents at pos
	int neighbours = landscape[agentPos].nAgents > 0 ? landscape[agentPos].nAgents : 1;

	// value of food shared with neighbours
	float food = static_cast<float> (landscape[agentPos].dFood) / (static_cast<float>(neighbours));

	// energy in if move is true - loop following is penalised
	agentEnergyVec[whichAgent] += food;
}

/// function to deplete landscape
// assign food as max - proportion of visits per generation
void depleteLand()
{
	for (int l = 0; l < landscape.size(); l++)
	{
		float foodDiff = (maxFood - (static_cast<float>(landscape[l].nTotAgents) / static_cast<float>(popsize * tMax)));
		landscape[l].dFood = foodDiff;
	}
}

/// func to reset agents on landscape in ecological time
// agents return to roost
void resetAgentsOverTime()
{
	for (int l = 0; l < landscape.size(); l++)
	{
		landscape[l].nAgents = 0;
	}
}

/// func to reset agents on landscape in evol time
void resetAgentsOverGens()
{
	for (int l = 0; l < landscape.size(); l++)
	{
		landscape[l].nTotAgents = 0;
	}
}

/// function to print landscape values
void printLand(const int& gen_p)
{
	// open or append
	std::ofstream landofs;
	landofs.open("landOut.csv", std::ofstream::out | std::ofstream::app);

	// col header
	if (gen_p == 0) { landofs << "gen,pos,food,visits\n"; }

	// print for each land cell
	{
		for (int landcell = 0; landcell < landscape.size(); landcell++)
		{
			landofs
				<< gen_p << ","
				<< landcell << ","
				<< landscape[landcell].dFood << ","
				<< landscape[landcell].nTotAgents << "\n";
		}
		//close
		landofs.close();
	}
}

// ends here
