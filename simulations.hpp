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

/* simulation wrapper */
/// do simulations
void do_simulation(std::vector<std::string> cli_args){
    // gather cli args
    const std::string type = cli_args[1];
    const int PHI = std::stoi(cli_args[2]);
    const float RHO = std::stof(cli_args[3]);
    const int genmax = std::stoi(cli_args[4]);
    const int timesteps = std::stoi(cli_args[5]);
    const float init_d = std::stof(cli_args[6]);
    std::string rep = cli_args[7];

    // init pop & landscape, force population D to high (1) low (0.1) med (0.5)
    std::vector<agent> pop (popsize);
    force_d(pop, init_d);
    landscape landscape_;
    landscape_.doMakeFood(PHI, RHO);

    std::string outfile = identify_outfile(type, PHI, RHO, timesteps, init_d, std::stoi(rep));

    assert(((type == "info") | (type == "noinfo")) && "sim type not available");

    if(type == "info"){

        evolve_pop_no_M(pop, genmax, timesteps, PHI, RHO, landscape_, outfile);
    }
    else{
        evolve_pop_no_info(pop, genmax, timesteps, PHI, RHO, landscape_, outfile);
    }

    std::cout << "pop evolved!" << "\n";
}

// ends here
