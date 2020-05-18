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
void homogenise_pop(std::vector<agent> &pop){
    // prep and calc population mean
    double pop_mean_a, pop_mean_b, pop_mean_Mf, pop_mean_D;
    pop_mean_a = pop[0].a;
    pop_mean_b = pop[0].b;
    pop_mean_Mf = pop[0].Mf;
    pop_mean_D = pop[0].D;
    for(size_t i_hp = 1; i_hp < pop.size(); i_hp ++)
    {
        pop_mean_a = (pop_mean_a + pop[i_hp].a) / 2.f;
        pop_mean_b = (pop_mean_b + pop[i_hp].b) / 2.f;
        pop_mean_Mf = (pop_mean_Mf + pop[i_hp].Mf) / 2.f;
        pop_mean_D = (pop_mean_D + pop[i_hp].D) / 2.f;
    }
    // prep for new vector of agents 95% of the size
    std::vector<agent> tmp_pop(popsize);
    agent tmp_agent;
    tmp_agent.a = pop_mean_a;
    tmp_agent.b = pop_mean_b;
    tmp_agent.Mf = pop_mean_Mf;
    tmp_agent.D = pop_mean_D;
    // assign new vector
    for(size_t i_hp = 0; i_hp < pop.size(); i_hp ++)
    {
     tmp_pop[i_hp] = tmp_agent;
    }
    assert(tmp_pop.size() == popsize && "homogenised pop is different size");
    std::swap(pop, tmp_pop);
    tmp_pop.clear();
}

/// function to introduce mutants
// remove 24 agents and put in 3 mutants each
// requires a HOMOGENISED POPULATION
void add_mutants(std::vector<agent> &pop){

    // get steps for mutation
    const std::vector<float> mut_steps = {-1.f, -0.5f, 0.f, 0.5f, 1.f};

    // get mutants
    std::vector<std::pair<float, float> > mut_vals;

    for(size_t i_mp = 0; i_mp < mut_steps.size(); i_mp++){
        for(size_t j_mp = 0; j_mp < mut_steps.size(); j_mp++){
            mut_vals.push_back({mut_steps[i_mp], mut_steps[j_mp]});
        }
    }

    // modify the first 25 (really 24) agents
    for(size_t i_mp = 0; i_mp < mut_vals.size(); i_mp++){
        pop[i_mp].a += mut_vals[i_mp].first;
        pop[i_mp].b += mut_vals[i_mp].second;
    }

}

/// 2d fitness landscape
// input a population, get the mean parameters, replicate them
// introduce some mutants and see how they perform
void get_fitness_landscapes(std::vector<agent> pop,
                            landscape &landscape,
                            const int timesteps,
                            const int fitland_reps,
                            const int leader_choices,
                            std::vector<std::string> output_path){
    std::vector<agent> tmp_pop = pop;
    class landscape tmp_landscape = landscape;

    std::vector<agent> to_print;

    // get the resident mean a and b
    const float res_mean_a = tmp_pop.end()->a;
    const float res_mean_b = tmp_pop.end()->b;

    for(int flr = 0; flr < fitland_reps; flr++){
        for(int t = 0; t < timesteps; t++){
            // do the foraging things here
            shufflePopSeq(tmp_pop);
            doFollowDynamic(tmp_pop, leader_choices);
            do_foraging_dynamic(tmp_landscape, tmp_pop);

            tmp_landscape = landscape;
        }

        // evaluate tmp pop differences here
//        bool got_resident = false;
        for(size_t i_hp = 0; i_hp < tmp_pop.size(); i_hp++)
        {
            // add the mutants
            if(tmp_pop[i_hp].a != res_mean_a || tmp_pop[i_hp].b != res_mean_b){
                to_print.push_back(tmp_pop[i_hp]);
            }
//            // add one resident
//            if(tmp_pop[i_hp].a == res_mean_a && tmp_pop[i_hp].b == res_mean_b && got_resident == false)
//            {
//                to_print.push_back(tmp_pop[i_hp]);
//                got_resident = true;
//            }

            // print csv of a, b, pf and fitness
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
    const int timesteps = std::stoi(cli_args[5]);
    const float init_d = std::stof(cli_args[6]);
    const int leader_choices = std::stoi(cli_args[7]);
    std::string rep = cli_args[8];

    // init pop & landscape, force population D to high (1) low (0.1) med (0.5)
    std::vector<agent> pop (popsize);
    force_d(pop, init_d);
    landscape landscape_;
    landscape_.doMakeFood(PHI, RHO);

    // prepare to write data
    //    prepare_data_folders(type);
    const std::vector<std::string> output_path = identify_outfile(type, PHI,
                                   RHO, timesteps, init_d, leader_choices, rep);

    assert(((type == "info") || (type == "noinfo")) && "sim type not available");

    // currently relies on side effects and does not save evolved population
    if(type == "info"){

        evolve_pop_yes_M(pop, genmax, timesteps, PHI, RHO, leader_choices,
                        landscape_, output_path);
    }
    else{
        evolve_pop_no_info(pop, genmax, timesteps, PHI, RHO, landscape_, output_path);
    }

    std::cout << "pop evolved!" << "\n";

    // if following allowed print fitness landscapes
    if(type == "info"){
        homogenise_pop(pop);
        add_mutants(pop);
        get_fitness_landscapes(pop, landscape_, 10, 10, leader_choices, output_path);
    }

    std::cout << "fitness landscape printed!\n";
}

// ends here
