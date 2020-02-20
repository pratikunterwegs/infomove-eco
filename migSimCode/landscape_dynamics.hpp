#pragma once

/// header controlling depletion dynamics
#include "params.hpp"
#include "agents.hpp"
#include <cmath>
#include <vector>

/// function to deplete landscape
// update dFood based on wrapped agent effect
// agent effect is specified by smootherstep above
void agent::depleteFood(landscape& landscape)
{
    // add to foragers
    //landscape.foragers[static_cast<size_t>(pos)] += 1;

    // get energy
    energy += landscape.resources[static_cast<size_t>(pos)]; /*/
            static_cast<float> (landscape.foragers[static_cast<size_t>(pos)]);*/
    energy -= predation_cost; /*/
            static_cast<float> (landscape.foragers[static_cast<size_t>(pos)]);*/

    // deplete landscape
    landscape.resources[static_cast<size_t>(pos)] -= maxDepletion;
}

/// wrapper function
int wrapper(int distance, int current_val, int max_val) {

	int new_pos = (max_val + current_val + distance) % max_val;
	return new_pos;
}

/// function to walk along the circle
void agent::circleWalk()
{
	// check where agent is
	assert(pos <= n_patches - 1 && "func circleWalk: agent now over max land!");
	assert(pos >= 0 && "func circleWalk: agent now over min land!");

    pos = wrapper(tradeOffParam, pos, n_patches);

    //landscape.foragers[static_cast<size_t>(pos)] += 1;

	assert(pos <= n_patches - 1 && "func circleWalk: agent walked over max land!");
	assert(pos >= 0 && "func circleWalk: agent walked over min land!");

}

/// function to implement the foraging dynamic with turns and tradeoff
void do_foraging_dynamic(landscape& landscape, std::vector<agent>& population, int turns)
{
    for (size_t turn_value = 0; static_cast<int>(turn_value) < turns; turn_value++)
    {
        // over as many iterations as foraging_turns agents choose
        // to explore or exploit
        for (size_t indiv = 0; indiv < popsize; indiv++)
        {
            population[indiv].circleWalk();
            population[indiv].depleteFood(landscape);

            // remember food at current pos
            population[indiv].mem_last_pos = landscape.resources[static_cast<size_t>(population[indiv].pos)];
        }
    }
}


// ends here
