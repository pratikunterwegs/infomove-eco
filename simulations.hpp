#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <chrono>


#include "params.hpp"
#include "landscape.hpp"
#include "agents.hpp"
#include "utilities.hpp"
#include "landscape_dynamics.hpp"

/// function to evolve population
std::vector<agent> evolve_pop_no_M(std::vector<agent> &pop,
  const int genmax, const int timesteps,
  const int PHI, const float RHO,
  landscape& landscape, std::string outfile)
  {

    std::cout << "evolving on PHI = " << PHI << " RHO = " << RHO << "\n";
    auto t1 = std::chrono::high_resolution_clock::now();
    // run over n agents
    for (int gen = 0; gen < genmax; gen++) {
      // shuffle population once per gen
      shufflePopSeq(pop);
      // loop through timesteps
      for (int t = 0; t < timesteps; t++) {
        doFollowDynamic(pop);
        do_foraging_dynamic(landscape, pop);
      }
      landscape.doMakeFood(PHI, RHO); // landscape replenish

      if((gen == 0) | (gen % epoch == 0) | (gen == genmax - 1)){
        print_agents(pop, gen, outfile);
      }

      do_reprod(pop, false); // do no evolve M
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << "pop evolve time: " << duration << "\n";

    return pop;
  }

  /// simulation without any following dynamic
  /// function to evolve population
  std::vector<agent> evolve_pop_no_info(std::vector<agent> &pop,
    const int genmax, const int timesteps,
    const int PHI, const float RHO,
    landscape& landscape, std::string outfile)
    {

      std::cout << "evolving on PHI = " << PHI << " RHO = " << RHO << "\n";
      auto t1 = std::chrono::high_resolution_clock::now();
      // run over n agents
      for (int gen = 0; gen < genmax; gen++) {
        // shuffle population once per gen
        shufflePopSeq(pop);
        // loop through timesteps
        for (int t = 0; t < timesteps; t++) {
          do_move_noinfo(pop);
          do_foraging_dynamic(landscape, pop);
        }
        landscape.doMakeFood(PHI, RHO); // landscape replenish

        if((gen == 0) | (gen % epoch == 0)){
          print_agents(pop, gen, outfile);
        }
          do_reprod(pop, false); // do no evolve M
      }
      auto t2 = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
      std::cout << "pop evolve time: " << duration << "\n";

      return pop;
    }

    // ends here
