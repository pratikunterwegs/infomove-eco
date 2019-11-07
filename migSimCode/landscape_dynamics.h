#pragma once

/// header controlling depletion dynamics
// to be imported after agents.h

#include "landscape.h"
#include "agents.h"
// make gridcell class
class gridcell
{
public:
	gridcell() : dFood(1.f), dPos(0.f) {};
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
		landscape[i].dPos = static_cast<float>(i) / maxLandPos;
	}
}

/// function for wrapped distance
float getWrappedDist(const float& x1, const float& x2, const float& x_max)
{
    float tempdist = abs(x2 - x1);
    assert(tempdist <= maxLandPos && "tempdist is too high");
    tempdist = std::min(tempdist, x_max - tempdist);
    return tempdist;
}

/// function to deplete landscape
// update dFood based on wrapped agent effect
void depleteFood(const int& whichAgent)
{
    for(int l = 0; l < landPoints; l++)
    {
        // wrapped distance from agent
        float dist = getWrappedDist(population[whichAgent].moveAngleCopy, landscape[l].dPos, maxLandPos);
        landscape[l].dFood -= maxFood/(maxFood + exp(depletionSlope * (dist - depletionRadius)));
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
    float dist_left = getWrappedDist(population[whichAgent].moveAngleCopy, landscape[bound_left].dPos, maxLandPos);
    float dist_right = getWrappedDist(population[whichAgent].moveAngleCopy, landscape[bound_right].dPos, maxLandPos);

    float food_left = landscape[bound_left].dFood;
    float food_right = landscape[bound_right].dFood;
    // agent foraging is interpolated
    agentEnergyVec[whichAgent] = ((food_left * dist_left) + (food_right * dist_right)) / (dist_left + dist_right);
    
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