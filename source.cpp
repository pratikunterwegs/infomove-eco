/// migSimCode.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <fstream>
#include <string>
#include <chrono>

#include "params.hpp"
#include "landscape.hpp"
#include "agents.hpp"
#include "landscape_dynamics.hpp"
#include "simulations.hpp"
#include "utilities.hpp"

// test wraparound
void test_wrap_distance()
{
    int wrapped_val;
    wrapped_val = wrap_distance(-1, 0, 100);
    assert(wrapped_val == 99);

    wrapped_val = wrap_distance(2, 99, 100);
    assert(wrapped_val == 1);
}


// test command line args
void test_cli_args(std::vector<std::string> cli_args) noexcept
{
    // check for size
    if(cli_args.size() != 7) // count RHO PHI gens timesteps turns rep
    {
        std::cerr << "wrong number of command args\n";
    }

    // check args content
    for (size_t arg = 1; arg < 3; arg++) {
        assert(std::stof(cli_args[arg]) > 0.f && "cli_args: negative!");
    }
    for (size_t arg = 3; arg < 5; arg++) {
        assert(std::stoi(cli_args[arg]) > 0.f && "cli_args: negative!");
    }
}

/// main function
int main(int argc, char* argv[])
{
    //basic tests
    test_wrap_distance();
    std::vector<std::string> cli_args(argv, argv+argc);
    test_cli_args(cli_args);

    // init gsl rng
    const gsl_rng_type * T;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);

    // gather cli args
    const float RHO = std::stof(cli_args[1]);
    const float PHI = std::stof(cli_args[2]);
    const int genmax = std::stoi(cli_args[3]);
    const int timesteps = std::stoi(cli_args[4]);
    const int turns = std::stoi(cli_args[5]);
    std::string rep = cli_args[6];

    // init pop and landscape
    std::vector<agent> pop (popsize);
    landscape landscape_;

    // do simulation
    std::vector<agent> evolved_pop = evolve_pop(pop, genmax, timesteps, turns, RHO, PHI, landscape_);

    // print evolved population params
    print_agents(evolved_pop, RHO, PHI, timesteps, turns, std::stoi(rep));

    // print landscape as well -- later
}

// end here
