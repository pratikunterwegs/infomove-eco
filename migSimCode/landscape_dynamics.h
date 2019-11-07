#pragma once

/// header controlling depletion dynamics
// to be imported after agents.h

#include "params.h"
#include "agents.h"
// make gridcell class
class gridcell
{
public:
	gridcell() : dFood(10.f), dPos(0.f) {};
	~gridcell() {};
	// each gridcell stores nAgents and food
	float dFood, dPos;
};

// init landscape of length landPoints
std::vector<gridcell> landscape(landPoints);

/// function to make positions
void makePositions(std::vector<gridcell>& landscape)
{
	for (int i = 0; i < landPoints; i++)
	{
		landscape[i].dPos = static_cast<float>(i) * maxLandPos / static_cast<float>(landPoints);
	}
}

/// function for wrapped distance
float getWrappedDist(const float& x1, const float& x2)
{
    float tempdist = abs(x2 - x1);
    assert(tempdist <= maxLandPos && "tempdist is beyond land");
    tempdist = std::min(tempdist, maxLandPos - tempdist);
    return tempdist;
}

/// function to deplete landscape
// update dFood based on wrapped agent effect
void depleteFood(const int& whichAgent)
{
    assert(population[whichAgent].moveAngleCopy >= 0 && "pop has neg moves");
    for(int l = 0; l < landPoints; l++)
    {
        // wrapped distance from agent
        float dist = getWrappedDist(population[whichAgent].moveAngleCopy, landscape[l].dPos);

        landscape[l].dFood -= (1/(depletionParam + exp(depletionSlope * (dist - depletionRadius))));
    }
}

/// function to get energy
void doGetFood(const int& whichAgent)
{
	// loop through landscape looking for pair of positions
    int l = 0;
    int bound_right = 0;
    while((l < landPoints))
    {
        // get right bound
        bound_right = landscape[bound_right].dPos > population[whichAgent].moveAngleCopy ? bound_right : l;
        l++;
    }

    // left bound is right bound - 1
    int bound_left = (bound_right - 1 >= 0)? (bound_right - 1): landPoints + (bound_right - 1);
    // energy is left bound / left distance + right bound / right distance
    float dist_left = getWrappedDist(population[whichAgent].moveAngleCopy, landscape[bound_left].dPos);
    float dist_right = getWrappedDist(population[whichAgent].moveAngleCopy, landscape[bound_right].dPos);

    float food_left = landscape[bound_left].dFood;
    float food_right = landscape[bound_right].dFood;
    // agent foraging is interpolated
    agentEnergyVec[whichAgent] += ((food_left * dist_left) + (food_right * dist_right)) / (dist_left + dist_right);
    
}

/// function to replenish food each generations
void doMakeFood()
{
	for (int l = 0; l < landPoints; l++)
	{
		landscape[l].dFood = maxFood;
	}
}
/// function to print landscape values
void printLand(const int& gen_p)
{
	// open or append
	std::ofstream landofs;
	landofs.open("dataLand.csv", std::ofstream::out | std::ofstream::app);
	// col header
	if (gen_p == 0) { landofs << "gen,pos,food\n"; }
	// print for each land cell
	{
		for (int landcell = 0; landcell < landscape.size(); landcell++)
		{
			landofs
				<< gen_p << ","
				<< landscape[landcell].dPos << ","
				<< landscape[landcell].dFood << "\n";
		}
		//close
		landofs.close();
	}
}

// ends here