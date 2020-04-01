#pragma once

/// header controlling depletion dynamics
#include "params.hpp"
#include "landscape.hpp"
#include "agents.hpp"
#include <cmath>
#include <vector>

/// wrap_distance function
int wrap_distance(int distance, int current_val, int max_val) {

    int new_pos = (max_val + current_val + distance) % max_val;
    return new_pos;
}

/// function to deplete landscape
void agent::deplete_and_move(landscape& landscape)
{
   size_t to_deplete = static_cast<size_t> (wrap_distance(-M, pos, n_patches));

    // get vector of assessed positions and food at those positions
    std::vector<int> vec_pos(static_cast<size_t>(2 * M + 1));

    for (int m = -M; m <= M; m++) {
        size_t tmp_p = static_cast<size_t>(wrap_distance(m, pos, n_patches));

        if(landscape.resources[tmp_p] > landscape.resources[to_deplete]){
            to_deplete = tmp_p;
        }
    }
    // deplete food from the highest position
    // should depletion happen before assessing averages on either side?
    energy += landscape.resources[to_deplete];
    // update memory
    mem_energy = landscape.resources[to_deplete];
    landscape.resources[to_deplete] = 0;

    // movement section -- changed 30/03/20
    // agent moves to depleted position
    pos = static_cast<int>(to_deplete);

}

/// function to implement the foraging dynamic
void do_foraging_dynamic(landscape& landscape, std::vector<agent>& population)
{
    for (size_t indiv = 0; indiv < popsize; indiv++)
    {
      population[indiv].deplete_and_move(landscape);
    }
}


// ends here
