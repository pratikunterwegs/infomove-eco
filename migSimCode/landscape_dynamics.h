#pragma once

/// header controlling depletion dynamics
// to be imported after agents.h

#include "params.h"
#include "agents.h"
#include <cmath>

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
		landscape[i].dPos = static_cast<float>(i) / static_cast<float>(landPoints);
	}
}

/// function for wrapped distance
float getWrappedDist(const float& x1, const float& x2)
{
    float tempdist = abs(x2 - x1);
    assert(tempdist <= 1.f && "tempdist is beyond land");
    tempdist = std::min(tempdist, 1.f - tempdist);
    return tempdist;
}

/// function for smootherstep
// take distance from agent, deadzone and deplzone
float smootherstep(float& x, const float& deadZone, const float& deplZone) {
	
	if (x < deadZone) { x = deadZone; }
	if (x > deplZone) { x = deplZone; }
	x = (x - deplZone) / (deadZone - deplZone);

	assert(deadZone <= deplZone && "smootherstep: deplZone gt deadZone");
	return x * x * x * (x * (x * 6 - 15) + 10);
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
// agent effect is specified by smootherstep above
void agent::depleteFood()
{
    assert(circPos >= 0.f && "func depleteFood: pop has neg moves");
    for(int l = 0; l < landPoints; l++)
    {
        // wrapped distance from agent
        float dist = getWrappedDist(circPos, landscape[l].dPos);
		float depleted = maxDepletion * landscape[l].dFood * smootherstep(dist, deadZone, deplZone);

		landscape[l].dFood -= (landscape[l].dFood - depleted) > 0.f ? depleted : landscape[l].dFood;

		assert(landscape[l].dFood >= 0.f && "func depleteFood: landscape food has become negative!");
    }    
}

/// function to get energy
void agent::doGetFood()
{
	// check where agent is
	assert(circPos <= 1.f && "func doGetFood: agent walked over max land!");
	assert(circPos >= 0.f && "func doGetFood: agent walked over min land!");
	// get right bound by rounding proportion on the landscape
    int bound_right = static_cast<int>(floor((circPos) * static_cast<float>(landPoints-1)));

    // left bound is right bound - 1
    int bound_left = (bound_right - 1 >= 0)? (bound_right - 1): landPoints + (bound_right - 1);

	// check bounds
	assert(bound_left >= 0 && "func doGetFood: left bound is negative");
	assert(bound_right <= (landPoints - 1) && "func doGetFood: right bound above max land points");

    // energy is left bound / left distance + right bound / right distance
    float dist_left = getWrappedDist(circPos, landscape[bound_left].dPos);
    float dist_right = getWrappedDist(circPos, landscape[bound_right].dPos);

    float food_left = landscape[bound_left].dFood;
    float food_right = landscape[bound_right].dFood;
    // agent foraging is interpolated
    energy += ((food_left * dist_left) + (food_right * dist_right)) / (dist_left + dist_right);

    // std::cout << "agent " << whichAgent << " energy = " << agentEnergyVec[whichAgent] << "\n";
    
}

/// function to walk along the circle
void agent::circleWalk()
{
	bool direction = walkDirection(rng);
	// check where agent is
	assert(circPos <= 1.f && "func circleWalk: agent now over max land!");
	assert(circPos >= 0.f && "func circleWalk: agent now over min land!");
	
	if (tradeOffParam < normDist(rng)) {
		circPos += direction ? (moveDist) : (-moveDist);

		circPos = fmod(circPos, 1.0f);

		circPos = circPos > 0.f ? circPos : 1.f + circPos;
	}
	assert(circPos <= 1.f && "func circleWalk: agent walked over max land!");
	assert(circPos >= 0.f && "func circleWalk: agent walked over min land!");

}

/// needs funcs memory

/// function to print landscape values
void printLand(const int& gen_p, const int& t_p)
{
	// open or append
	std::ofstream landofs;
	landofs.open("dataLand.csv", std::ofstream::out | std::ofstream::app);
	// col header
	if ((gen_p == 0) && (t_p == 0)) { landofs << "gen,t,pos,food\n"; }
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
