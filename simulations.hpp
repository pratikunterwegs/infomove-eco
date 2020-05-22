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

/// function to make homogenous population with mean values
void homogenise_pop(std::vector<agent> &pop,
                    const float a_h,
                    const float b_h,
                    const float Mf_h){
    // reset the a, b, M and Mf of the population
    for(size_t i_hp = 1; i_hp < pop.size(); i_hp ++)
    {
        pop[i_hp].a = a_h;
        pop[i_hp].b = b_h;
        pop[i_hp].Mf = Mf_h;
        pop[i_hp].M = static_cast<int>(Mf_h);
    }
}

/// a fuction to add a crossing functionality as in tidyr
/// currently supports pairs of float values
std::vector<std::pair<float, float> > make_combinations(std::vector<float> vals1,
                                                        std::vector<float> vals2,
                                                        const int n_reps)
{
    // get mutants
    std::vector<std::pair<float, float> > combo_vals;

    for(size_t i_mp = 0; i_mp < vals1.size(); i_mp++){
        for(size_t j_mp = 0; j_mp < vals2.size(); j_mp++){

            // add a number of replicates of each combination
            for(int n_reps_ = 0; n_reps_ < n_reps; n_reps_++){
                combo_vals.push_back({vals1[i_mp], vals2[j_mp]});
            }
        }
    }

    // check for size
    assert(combo_vals.size() == vals1.size()*vals2.size()*n_reps &&
           "combo vals: incorrect number of combinations");
    return combo_vals;
}

/// function to introduce mutants
void add_mutants(std::vector<agent> &pop,
                 const float a_mut,
                 const float b_mut)
{
    // simply replace 25 individuals with ONE mutant value
    for(int a_m = 0; a_m < 25; a_m++)
    {
        pop[a_m].a = a_mut;
        pop[a_m].b = b_mut;
    }

}

/// 2d fitness landscape
// requires a homogenised and mutant inserted population
void get_fitness_landscape(std::string type,
                           std::vector<agent> &pop,
                           landscape &landscape,
                           const int timesteps,
                           const int leader_choices,
                           std::vector<std::string> output_path,
                           const int mut_combo,
                           const int replicate){
    class landscape tmp_landscape = landscape;

    for(int t = 0; t < timesteps; t++){
        // do the foraging things here
        shufflePopSeq(pop);

        // move with or without info based on sim type
        if(type == "noinfo")
        {
            do_move_noinfo(pop);
        }
        else {
            doFollowDynamic(pop, leader_choices);
        }
        do_foraging_dynamic(tmp_landscape, pop);

        tmp_landscape = landscape;
    }

    print_fitness_landscape(pop, output_path, mut_combo, replicate);
}

/* simulation wrapper */
/// do simulations
void do_simulation(std::vector<std::string> cli_args){
    // gather cli args
    const std::string type = cli_args[1];
    const int PHI = std::stoi(cli_args[2]);
    const float RHO = std::stof(cli_args[3]);
    const int genmax = std::stoi(cli_args[4]);
    const int timesteps = std::stoi(cli_args[5]); // IN ECO THE TIMESTEPS SHOULD START AT 500
    const float init_d = std::stof(cli_args[6]);
    const int leader_choices = std::stoi(cli_args[7]);
    std::string rep = cli_args[8];

    assert(((type == "info") || (type == "noinfo")) && "sim type not available");

    // prepare to write data
    const std::vector<std::string> output_path = identify_outfile(type, PHI,
                                   RHO, timesteps, init_d, leader_choices, rep);

    // init pop & landscape
    std::vector<agent> pop (popsize);
    landscape landscape_;
    landscape_.doMakeFood(PHI, RHO);

    // get a sequence of a and b to init the pop
    // THESE ARE CURRENTLY HARDCODED
    // THE INCREMENT IS ALSO THE GRADIENT LATER ON
    std::vector<float> vec_a, vec_b;
    const float increment = 0.5f;
    const float limit = 3.f;

    // assign vecs a and b
    for(float i = -limit; i <= limit; i+=increment)
    {
        vec_a.push_back(i);
    } {vec_b = vec_a;}

    // make combinations of a and b
    std::vector<std::pair<float, float> > init_params = make_combinations(vec_a, vec_b, 1);

    // homogenise the population to the values given in init_params
    for(size_t this_combo = 0; this_combo < init_params.size(); this_combo++)
    {
        std::cout << "Fitness landscape w/ init params: "
                  << init_params[this_combo].first
                  << " "
                  << init_params[this_combo].second
                  << "\n";
        homogenise_pop(pop, init_params[this_combo].first, init_params[this_combo].second, 2.f);
        add_mutants(pop, increment);
        get_fitness_landscape(type, pop, landscape_, timesteps, 20, leader_choices, output_path,
                              this_combo);
    }

    std::cout << "fitness landscape printed!\n";
}

// ends here
