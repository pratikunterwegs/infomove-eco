#pragma once

/// header controlling depletion dynamics
// to be imported after agents.h

#include "landscape.h"
#include "agents.h"

// make gridcell class
class gridcell
{
public:
	gridcell() : dFood(1.f), dPos {};
	~gridcell() {};

	// each gridcell stores nAgents and food
	float dFood, dPos;

};

// init landscape of length maxlandvec
std::vector<gridcell> landscape(maxLandVec);

// assign positions
for(int i = 0; i < maxLandVec; i++)
{
    landscape[i].dPos = static_cast<float>(i) / maxLandPos;
}

/// function for wrapped distance
float getWrappedDist(const float& x1, const float& x2, const float& x_max)
{
    const float tempdist = abs(x2 - x1);

    assert(tempdist <= maxLandPos && "tempdist is too high");

    tempdist = std::min(tempdist, x_max - tempdist);

    return tempdist;
}

/// function to deplete landscape
// update dFood based on wrapped agent effect
void depleteFood(const int& whichAgent)
{
    for(int l = 0; l < maxLandVec; l++)
    {
        // wrapped distance from agent
        float dist = getWrappedDist(population[i].moveAngleCopy, landscape[l].dPos, maxLandPos);

        dFood -= (1.f/(1.f + dist));
    }
}

/// function to get energy
void doGetFood(const int& whichAgent)
{
	// loop through landscape looking for pair of positions
    int l = 0;
    int bound_right = 0;
    while((l < maxLandVec))
    {
        // get right bound
        bound_right = landscape[bound_right].dPos > population[whichAgent].moveAngleCopy ? bound_right : l;
        
        l++;
    }

    // left bound is right bound - 1
    int bound_left = (bound_right - 1 >= 0)? (bound_right - 1): maxLandVec + (bound_right - 1);

    // energy is left bound / left distance + right bound / right distance

    float dist_left = getWrappedDist(population[whichAgent].moveAngleCopy, landscape[bound_left].dPos, maxLandPos);
    float dist_right = getWrappedDist(population[whichAgent].moveAngleCopy, landscape[bound_right].dPos, maxLandPos);

    float food_left = landscape[bound_left].dFood;
    float food_right = landscape[bound_right].dFood;

    // agent foraging is interpolated
    population[whichAgent] = ((food_left * dist_left) + (food_right * dist_right)) / (dist_left + dist_right);
    
}

// ends here