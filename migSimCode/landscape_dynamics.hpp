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

void agent::goToLandscape(landscape& landscape)
{
    landscape.foragers[static_cast<size_t>(pos)] += 1;
}

/// function to walk along the circle
void agent::circleWalk(landscape& landscape)
{
	// check where agent is
	assert(pos <= n_patches - 1 && "func circleWalk: agent now over max land!");
	assert(pos >= 0 && "func circleWalk: agent now over min land!");

	// move up the peak
	int pos_left = wrapper(-1, pos, n_patches);
	int pos_right = wrapper(1, pos, n_patches);

	// remove agent from previous cell
    landscape.foragers[static_cast<size_t>(pos)] -= 1;
    assert(landscape.foragers[static_cast<size_t>(pos)] >= 0
            && "func circleWalk: cell has neg agents!");

    pos = (landscape.resources[static_cast<size_t>(pos)] >
            landscape.resources[static_cast<size_t>(pos)]) ? pos_left : pos_right;

    landscape.foragers[static_cast<size_t>(pos)] += 1;

	assert(pos <= n_patches - 1 && "func circleWalk: agent walked over max land!");
	assert(pos >= 0 && "func circleWalk: agent walked over min land!");

}

/// function to choose explore or exploit
void agent::exploreOrExploit(landscape& landscape)
{
    if (tradeOffParam <= tradeoff_picker(rng))
	{
        circleWalk(landscape);
		total_distance += 1;
		// deplete energy
		energy -= move_cost;
		// energy = energy >= 0.000001f ? energy : 0.000001f;
	}
	else
	{
        depleteFood(landscape);
	}

	// remember food at current pos
    mem_last_pos = landscape.resources[static_cast<size_t>(pos)];
}


/// function to implement the foraging dynamic with turns and tradeoff
void do_foraging_dynamic(landscape& landscape, std::vector<agent>& population, int turns)
{
	// all agents go to landscape
	for (size_t indiv = 0; indiv < popsize; indiv++)
	{
        population[indiv].goToLandscape(landscape);
	}

    for (size_t turn_value = 0; static_cast<int>(turn_value) < turns; turn_value++)
	{
		// over as many iterations as foraging_turns agents choose
		// to explore or exploit
		for (size_t indiv = 0; indiv < popsize; indiv++)
		{
            population[indiv].exploreOrExploit(landscape);
		}
	}
}


// ends here
