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

// get one resident and all mutants and print only those
std::vector<agent> get_muts_and_resident(std::vector<agent> &pop,
                                 const float res_a,
                                 const float res_b){
    // which agents to return
    std::vector<agent> vec_mutants(25);
    // keep this resident
    agent one_resident;

    int i_wtm = 0; int i_res = 0;
    for(size_t i_sp = 0; i_sp < pop.size(); i_sp++)
    {
        if(pop[i_sp].a != res_a || pop[i_sp].b != res_b){
            vec_mutants[i_wtm] = pop[i_sp];
            i_wtm++;
            assert(i_wtm <= 25 && "get_muts_res: too many agents added, see float comparison");
        }
        else if (i_res == 0) {
            one_resident = pop[i_sp];
        }
    }
    vec_mutants.push_back(one_resident);
    assert(vec_mutants.size() == 26 && "get_muts_res: wrong number of agents");
    return vec_mutants;
}

// scale the energy of mutants by the resident
// the RESIDENT IS KNOWN TO BE THE LAST ELEMENT
void scale_energy(std::vector<agent> &pop)
{
    for(size_t i_se = 0; i_se < pop.size(); i_se++)
    {
        pop[i_se].energy -= pop.back().energy;
    }
}

/// 2d fitness landscape
// requires a homogenised and mutant inserted population
void get_fitness_landscape(std::string type,
                           std::vector<agent> &pop,
                           landscape &landscape,
                           const int timesteps,
                           const int leader_choices){
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
}

/* simulation wrapper */
/// do simulations
void do_simulation(std::vector<std::string> cli_args){
    // gather cli args
    const std::string type = cli_args[1];
    const int PHI = std::stoi(cli_args[2]);
    const float RHO = std::stof(cli_args[3]);
    const int timesteps = std::stoi(cli_args[4]); // IN ECO THE TIMESTEPS SHOULD START AT 500
    const float a_res = std::stof(cli_args[5]);
    const float b_res = std::stof(cli_args[6]);
    const int leader_choices = std::stoi(cli_args[7]);
    const float gradient = std::stof(cli_args[8]);
    const int n_rep = std::stoi(cli_args[9]);

    assert(((type == "info") || (type == "noinfo")) && "sim type not available");
    assert(gradient > 0.f && "gradient is 0 or negative");

    // print a, b values
    std::cout << "Fitness landscape w/ init params: "
           << "a = " << a_res << " "
           << "b = " << b_res << " "
           << "\n";

    // prepare to write data
    const std::vector<std::string> output_path = identify_outfile_eco(type, PHI,
                                   RHO, timesteps, a_res, b_res, leader_choices);
    std::cout << "outfile made\n";

    // init pop & landscape
    std::vector<agent> pop (popsize);
    std::vector<agent> tmp_pop = pop;
    landscape landscape_;
    landscape_.doMakeFood(PHI, RHO);

    // get a sequence of a and b to mutate the pop
    std::vector<float> vec_a_mut;
    std::vector<float> vec_b_mut;

    // make vectors of a_mut and b_mut
    for (float i = -gradient; i <= gradient; i+= gradient){
        std::cout << "increment = " << i << "\n";
        vec_a_mut.push_back(a_res+i);
        vec_b_mut.push_back(b_res+i);
    }

    // make combinations
    std::vector<std::pair<float, float> > mut_vals = make_combinations(vec_a_mut,
                                                                       vec_b_mut,
                                                                       1);

    std::cout << "passed gradient preparation\n";
    // homogenise the population to the values given in init_params
    for(size_t it2 = 0; it2 < mut_vals.size(); it2++){
        for(size_t jt2 = 0; jt2 < static_cast<size_t>(n_rep); jt2++){

            homogenise_pop(tmp_pop, a_res, b_res, 2.f);
            add_mutants(tmp_pop, mut_vals[it2].first, mut_vals[it2].second);
            get_fitness_landscape(type, tmp_pop, landscape_,
                                  timesteps, leader_choices);
            // summarise here
            std::vector<agent> summary_pop = get_muts_and_resident(tmp_pop, a_res, b_res);

            scale_energy(summary_pop);
            // print here when pop is subsampled
            print_fitness_landscape(summary_pop, output_path, it2, jt2);
            tmp_pop = pop;
        }
    }

    std::cout << "fitness landscape printed!\n";
}

// ends here
