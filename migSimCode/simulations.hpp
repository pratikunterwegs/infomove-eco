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
std::vector<agent> evolve_pop(const int genmax, const int timesteps,
                              const int foraging_turns,
                              const float RHO, const float PHI,
                              landscape& landscape)
{

    std::cout << "evolving on PHI = " << PHI << " RHO = " << RHO << "\n";
    //Initialization of pop and landscape
    std::vector<agent> pop (popsize);

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
//std::vector<agent> regime_shift(std::vector<agent> pop, landscape& landscape,
//                                const int genmax_shift, const int timesteps,
//                              const int foraging_turns, float new_R)
//{
//    //select a new R
//    new_R = powf(10.f, new_R);

//    // run over n agents
//    for (int gen= 0; gen < genmax_shift; gen++) {
//        std::cout << "gen = " << gen << "\n";
//        // loop through timesteps
//        for (int t = 0; t < timesteps; t++) {
//            // shuffle population
//            shufflePopSeq(pop);
//            // reset leaders
//            for (size_t ind = 0; static_cast<int>(ind) < popsize; ind++) {
//                doFollowDynamic(pop);
//                do_foraging_dynamic(landscape, pop, foraging_turns);
//            }
//        }
//        float regrowth = std::sin((2.f * static_cast<float>(M_PIl) *
//                                    static_cast<float>(static_cast<float>(gen))) /
//                                   (new_R));
//        landscape.doMakeFood(regrowth);
//        do_reprod(pop);

//    }

//    std::cout << "regime shifted...\n";
//    return pop;
//}

// ends here
