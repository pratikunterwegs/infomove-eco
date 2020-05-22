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
void evolve_pop_no_M(std::vector<agent> &pop,
                                   const int genmax, const int timesteps,
                                   const int PHI, const float RHO,
                                   const int leader_choices,
                                   landscape& landscape,
                                   std::vector<std::string> output_path)
{
    // create temp landscape to be used in this sim
    class landscape tmp_landscape = landscape;

    std::cout << "evolving on PHI = " << PHI << " RHO = " << RHO << "\n";
    auto t1 = std::chrono::high_resolution_clock::now();
    // run over n agents
    for (int gen = 0; gen < genmax; gen++) {

        // loop through timesteps
        for (int t = 0; t < timesteps; t++) {
            // shuffle population once per gen
            shufflePopSeq(pop);
            doFollowDynamic(pop, leader_choices);
            do_foraging_dynamic(tmp_landscape, pop);

            // print agents at certain time steps{
            if((gen == 0) || (gen % epoch == 0) || (gen == genmax - 1))
            {
                if((t < 5) || (t >= 50 && t <= 55) ||
                        (t >= 95)){
                    print_agent_data(pop, gen, t, output_path);
                }
            }
            // restore landscape to restored landscape
            tmp_landscape = landscape;
        }
        if((gen == 0) || (gen % epoch == 0) || (gen == genmax - 1)){
            print_agent_summary(pop, gen, output_path);
        }

        do_reprod(pop, false); // do no evolve M
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << "pop evolve time: " << duration << "\n";

}

/// function to evolve population
void evolve_pop_yes_M(std::vector<agent> &pop,
                                   const int genmax, const int timesteps,
                                   const int PHI, const float RHO,
                                   const int leader_choices,
                                   landscape& landscape,
                                   std::vector<std::string> output_path)
{
    // create temp landscape to be used in this sim
    class landscape tmp_landscape = landscape;

    std::cout << "evolving on PHI = " << PHI << " RHO = " << RHO << "\n";
    auto t1 = std::chrono::high_resolution_clock::now();
    // run over n agents
    for (int gen = 0; gen < genmax; gen++) {

        // loop through timesteps
        for (int t = 0; t < timesteps; t++) {
            // shuffle population once per gen
            shufflePopSeq(pop);
            doFollowDynamic(pop, leader_choices);
            do_foraging_dynamic(tmp_landscape, pop);

            // print agents at certain time steps{
            if((gen == 0) || (gen % epoch == 0) || (gen == genmax - 1))
            {
                if((t < 5) || (t >= 50 && t <= 55) ||
                        (t >= 95)){
                    print_agent_data(pop, gen, t, output_path);
                }
            }

            tmp_landscape = landscape;
        }
        if((gen == 0) || (gen % epoch == 0) || (gen == genmax - 1)){
            print_agent_summary(pop, gen, output_path);
        }

        do_reprod(pop, true); // do no evolve M
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << "pop evolve time: " << duration << "\n";
}

/// simulation without any following dynamic
/// function to evolve population
void evolve_pop_no_info(std::vector<agent> &pop,
                                      const int genmax, const int timesteps,
                                      const int PHI, const float RHO,
                                      landscape& landscape,
                                      std::vector<std::string> output_path)
{
    // create temp landscape to be used in this sim
    class landscape tmp_landscape = landscape;

    std::cout << "evolving on PHI = " << PHI << " RHO = " << RHO << "\n";
    auto t1 = std::chrono::high_resolution_clock::now();
    // run over n agents
    for (int gen = 0; gen < genmax; gen++) {

        // loop through timesteps
        for (int t = 0; t < timesteps; t++) {
            // shuffle population once per gen
            shufflePopSeq(pop);
            do_move_noinfo(pop);
            do_foraging_dynamic(tmp_landscape, pop);
            // print agents at certain time steps{
            if((gen == 0) || (gen % epoch == 0) || (gen == genmax - 1))
            {
                if((t < 5) || (t >= 50 && t <= 55) ||
                        (t >= 95)){
                    print_agent_data(pop, gen, t, output_path);
                }
            }
            tmp_landscape = landscape;
        }
        if((gen == 0) || (gen % epoch == 0) || (gen == genmax -1)){
            print_agent_summary(pop, gen, output_path);
        }
        do_reprod(pop, true); // do no evolve M
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << "pop evolve time: " << duration << "\n";
}

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
// remove 24 agents and put in 3 mutants each
// requires a HOMOGENISED POPULATION
void add_mutants(std::vector<agent> &pop,
                 const float gradient){

    // get the resident strategy
    const float a_res = pop[0].a;
    const float b_res = pop[0].b;
    const float Mf_res = pop[0].Mf; // not really used now

    // check gradient positive tho it doesn't really matter
    assert(gradient >= 0.f && "add mutants: gradient must be positive");

    // get steps for mutation
    const std::vector<float> mut_steps = {-gradient, 0.f, gradient};

    std::vector<std::pair<float, float> >mut_vals = make_combinations(mut_steps, mut_steps, 3);

    // check for correct number of mutant combinations
    // really check length of mut_vals, which is 27
    assert(mut_vals.size() == static_cast<size_t>(27) && "add mutants: wrong number of mutant combinations");

    // modify the first 27 (really 24) agents
    // ONLY MODIFYING a AND b FOR NOW
    for(size_t i_mp = 0; i_mp < mut_vals.size(); i_mp++){
        pop[i_mp].a += mut_vals[i_mp].first;
        pop[i_mp].b += mut_vals[i_mp].second;
    }

    // check the number of mutants is really 24
    int n_mutants = 0;
    for(size_t i_mp = 0; i_mp < mut_vals.size(); i_mp++){
        if(pop[i_mp].a != a_res || pop[i_mp].b != b_res)
        {
            n_mutants++;
        }
    }
    assert(n_mutants == 24 && "add mutants: wrong number of mutants added");

}

/// 2d fitness landscape
// requires a homogenised and mutant inserted population
void get_fitness_landscape(std::string type,
                           std::vector<agent> pop,
                           landscape &landscape,
                           const int timesteps,
                           const int fitland_reps,
                           const int leader_choices,
                           std::vector<std::string> output_path){
    std::vector<agent> tmp_pop = pop;
    class landscape tmp_landscape = landscape;

    std::vector<agent> to_print;

    // get the resident strategy
    const float a_res = tmp_pop.end()->a;
    const float b_res = tmp_pop.end()->b;
    const float M_res = tmp_pop.end()->M; // not used yet

    for(int flr = 0; flr < fitland_reps; flr++){
        for(int t = 0; t < timesteps; t++){
            // do the foraging things here
            shufflePopSeq(tmp_pop);

            // move with or without info based on sim type
            if(type == "noinfo")
            {
                do_move_noinfo(tmp_pop);
            }
            else {
                doFollowDynamic(tmp_pop, leader_choices);
            }
            do_foraging_dynamic(tmp_landscape, tmp_pop);

            tmp_landscape = landscape;
        }

        // evaluate tmp pop differences here
        for(size_t i_hp = 0; i_hp < tmp_pop.size(); i_hp++)
        {
            // add the mutants
            if(tmp_pop[i_hp].a != a_res || tmp_pop[i_hp].b != b_res){
                to_print.push_back(tmp_pop[i_hp]);
            }

            // print csv of a, b, pf and energy
            print_fitness_landscape(to_print, output_path, flr);
            to_print.clear();
        }
        // restore temp pop to pop
        tmp_pop = pop;
    }
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
    const float increment = 0.25f;
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
        homogenise_pop(pop, init_params[this_combo].first, init_params[this_combo].second, 2.f);
        add_mutants(pop, increment);
        get_fitness_landscape(type, pop, landscape_, timesteps, 20, leader_choices, output_path);
    }

    std::cout << "fitness landscape printed!\n";
}

// ends here
