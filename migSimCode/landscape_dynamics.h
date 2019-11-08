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

/// function to replenish food each generations
void doMakeFood()
{
	for (int l = 0; l < landPoints; l++)
	{
		landscape[l].dFood = maxFood;
	}
}

/// function to deplete landscape
// update dFood based on wrapped agent effect
void depleteFood(const int& whichAgent)
{
    assert(population[whichAgent].circPos >= 0 && "pop has neg moves");
    for(int l = 0; l < landPoints; l++)
    {
        // wrapped distance from agent
        float dist = getWrappedDist(population[whichAgent].circPos, landscape[l].dPos);
		float depleted = (maxDepletion / (1.f + exp(depletionSlope * (dist - depletionRadius))));

		landscape[l].dFood -= (landscape[l].dFood - depleted) > 0.f ? depleted : landscape[l].dFood;

		assert(landscape[l].dFood >= 0.f && "landscape food has become negative!");
    }    
}

/// function to get energy
void doGetFood(const int& whichAgent)
{
	// loop through landscape looking for pair of positions
    int l = 0;
    int bound_right = static_cast<int>(ceil((population[whichAgent].circPos / maxLandPos) * static_cast<float>(landPoints)));

    // left bound is right bound - 1
    int bound_left = (bound_right - 1 >= 0)? (bound_right - 1): landPoints + (bound_right - 1);
    // energy is left bound / left distance + right bound / right distance
    float dist_left = getWrappedDist(population[whichAgent].circPos, landscape[bound_left].dPos);
    float dist_right = getWrappedDist(population[whichAgent].circPos, landscape[bound_right].dPos);

    float food_left = landscape[bound_left].dFood;
    float food_right = landscape[bound_right].dFood;
    // agent foraging is interpolated
    agentEnergyVec[whichAgent] += ((food_left * dist_left) + (food_right * dist_right)) / (dist_left + dist_right);

    // std::cout << "agent " << whichAgent << " energy = " << agentEnergyVec[whichAgent] << "\n";
    
}

/// function to walk along the circle
void circleWalkAndLearn(const int& whichAgent)
{
	bool direction = walkDirection(rng);
	// save old pos, bound, and value
	float oldPos = population[whichAgent].circPos;
	int oldPosBound = static_cast<int>(ceil((oldPos / maxLandPos) * static_cast<float>(landPoints)));
	float oldVal = landscape[oldPosBound].dFood;

	// move agent left or right
	population[whichAgent].circPos += (population[whichAgent].circWalkDist * (direction ? 1.f : -1.f));

	// new position bound
	int newPosBound = static_cast<int>(ceil((population[whichAgent].circPos / maxLandPos) * static_cast<float>(landPoints)));
	float newVal = landscape[newPosBound].dFood;

	// change angle if new val better than old val
	if (newVal > oldVal)
	{
		population[whichAgent].moveAngle = convertPosToAngle(population[whichAgent].circPos);
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