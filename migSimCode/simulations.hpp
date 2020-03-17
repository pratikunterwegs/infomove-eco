#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <chrono>


#include "params.hpp"
#include "ann/rnd.hpp"
#include "ann/ann2.hpp"
#include "landscape.hpp"
#include "agents.hpp"
#include "landscape_dynamics.hpp"

/// function to evolve population
std::vector<agent> evolve_pop(std::vector<agent> &pop,
                            const int genmax, const int timesteps,
                            const int foraging_turns,
                            const float RHO, const float PHI,
                            landscape& landscape)
{

    std::cout << "evolving on PHI = " << PHI << " RHO = " << RHO << "\n";

    // run over n agents
    for (int gen = 0; gen < genmax; gen++) {
        auto t1 = std::chrono::high_resolution_clock::now();
        std::cout << "gen = " << gen << " ";

        // shuffle population once per gen
        shufflePopSeq(pop);
        // loop through timesteps
        for (int t = 0; t < timesteps; t++) {
            std::cout << "time = " << t << "\n";
            // reset leaders
            for (size_t ind = 0; static_cast<int>(ind) < popsize; ind++) {
                doFollowDynamic(pop);
                do_foraging_dynamic(landscape, pop, foraging_turns);
            }
        }

        landscape.doMakeFood(RHO, PHI, DELTA);
        do_reprod(pop);

        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        std::cout << duration << "ms\n";

    }

    std::cout << "pop evolved...\n";
    return pop;
}

/// function to change regime
/// function to evolve population
std::vector<agent> regime_shift(std::vector<agent> pop, const int genmax_shift, 
                                const int timesteps, const int foraging_turns, 
                                float new_RHO, float new_PHI, landscape& landscape)
{
   std::cout << "regime shifted...\n";
   return pop;
}

// ends here
