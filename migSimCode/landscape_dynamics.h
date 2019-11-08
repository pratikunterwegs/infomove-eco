#pragma once

/// header controlling depletion dynamics
// to be imported after agents.h

#include "params.h"
#include "agents.h"
#include <math.h>

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
    assert(population[whichAgent].circPos >= 0.f && "func depleteFood: pop has neg moves");
    for(int l = 0; l < landPoints; l++)
    {
        // wrapped distance from agent
        float dist = getWrappedDist(population[whichAgent].circPos, landscape[l].dPos);
		float depleted = (maxDepletion / (1.f + exp(depletionSlope * (dist - depletionRadius))));

		landscape[l].dFood -= (landscape[l].dFood - depleted) > 0.f ? depleted : landscape[l].dFood;

		assert(landscape[l].dFood >= 0.f && "func depleteFood: landscape food has become negative!");
    }    
}

/// function to get energy
void doGetFood(const int& whichAgent)
{
	// check where agent is
	assert(population[whichAgent].circPos <= maxLandPos && "func doGetFood: agent walked over max land!");
	assert(population[whichAgent].circPos >= 0.f && "func doGetFood: agent walked over min land!");
	// get right bound by rounding proportion on the landscape
    int bound_right = static_cast<int>(floor((population[whichAgent].circPos / maxLandPos) * static_cast<float>(landPoints)));

    // left bound is right bound - 1
    int bound_left = (bound_right - 1 >= 0)? (bound_right - 1): landPoints + (bound_right - 1);

	// check bounds
	assert(bound_left >= 0 && "func doGetFood: left bound is negative");
	assert(bound_right <= (landPoints - 1) && "func doGetFood: right bound above max land points");

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
	// check where agent is
	assert(population[whichAgent].circPos <= maxLandPos && "func circleWalk: agent walked over max land!");
	assert(population[whichAgent].circPos >= 0.f && "func circleWalk: agent walked over min land!");
	// save old pos, bound, and value
	float oldPos = population[whichAgent].circPos;
	int oldPosBound = static_cast<int>(floor((oldPos / maxLandPos) * static_cast<float>(landPoints)));
	// check if bounds are too high
	assert(oldPosBound <= landPoints && "func circleWalk: forage pos beyond landscape");
	float oldVal = landscape[oldPosBound].dFood;

	// move agent left or right
	// HANDLE TO ENSURE AGENT STAYS ON WRAPPED LANDSCAPE
	float moveDist = (population[whichAgent].circWalkDist * (direction ? 1.f : -1.f));
	population[whichAgent].circPos += moveDist;
	population[whichAgent].circPos = fmod( (maxLandPos+population[whichAgent].circPos), maxLandPos);


	assert(population[whichAgent].circPos <= maxLandPos && "func circleWalk: agent newpos over max land!");
	assert(population[whichAgent].circPos >= 0.f && "func circleWalk: agent newpos over min land!");

	// new position bound
	int newPosBound = static_cast<int>(floor((population[whichAgent].circPos / maxLandPos) * static_cast<float>(landPoints)));
	float newVal = landscape[newPosBound].dFood;

	// change angle if new val better than old val
	if (newVal > oldVal)
	{
		population[whichAgent].moveAngle = convertPosToAngle(population[whichAgent].circPos);
	}
}

/// function to print landscape values
void printLand(const int& gen_p, const int& t_p)
{
	// open or append
	std::ofstream landofs;
	landofs.open("dataLand.csv", std::ofstream::out | std::ofstream::app);
	// col header
	if (gen_p == 0) { landofs << "gen,t,pos,food\n"; }
	// print for each land cell
	{
		for (int landcell = 0; landcell < landscape.size(); landcell++)
		{
			landofs
				<< gen_p << ","
				<< t_p << ","
				<< landscape[landcell].dPos << ","
				<< landscape[landcell].dFood << "\n";
		}
		//close
		landofs.close();
	}
}

// ends here