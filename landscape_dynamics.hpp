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

   float avg_left = 0.f;
   float avg_right = 0.f;
   float curr_pos = landscape.resources[static_cast<size_t>(pos)];

    // get vector of assessed positions and food at those positions
    std::vector<int> vec_pos(static_cast<size_t>(2 * M + 1));

    for (int m = -M; m <= M; m++) {
        size_t tmp_p = static_cast<size_t>(wrap_distance(m, pos, n_patches));

        if (m > 0){
            avg_right += landscape.resources[tmp_p];
        }
        if (m < 0){
            avg_left += landscape.resources[tmp_p];
        }

        if(landscape.resources[tmp_p] > landscape.resources[to_deplete]){
            to_deplete = tmp_p;
        }
    }

    // get real averages
    avg_left = avg_left/static_cast<float>(M);
    avg_right = avg_right/static_cast<float>(M);

    // deplete food from the highest position
    // should depletion happen before assessing averages on either side?
    energy += landscape.resources[to_deplete];
    // update memory
    mem_energy = landscape.resources[to_deplete];
    landscape.resources[to_deplete] = 0;

    // movement section
    if(avg_right > avg_left && avg_right > curr_pos){ pos = wrap_distance(1, pos, n_patches); }
    else if (avg_left > avg_right && avg_left > curr_pos) { pos = wrap_distance(-1, pos, n_patches);  }

}

/// function to implement the foraging dynamic with turns and tradeoff
void do_foraging_dynamic(landscape& landscape, std::vector<agent>& population, int turns)
{
    for (size_t turn_value = 0; static_cast<int>(turn_value) < turns; turn_value++)
    {
//        std::cout << "turn = " << turn_value << "\n";
        // over as many iterations as foraging_turns agents choose
        // to explore or exploit
        for (size_t indiv = 0; indiv < popsize; indiv++)
        {
            population[indiv].deplete_and_move(landscape);
        }
    }
}


// ends here
