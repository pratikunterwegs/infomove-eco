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

/// evolution dynamics function
int do_main()
{
    // init new landscape
    landscape landscape;
    std::vector<agent> evolved_pop (popsize);

    float RHO = 1.f;
    float PHI = 1.f;

    evolved_pop = evolve_pop(evolved_pop, genmax, tMax, foraging_turns,
                                                RHO, PHI, landscape);

    std::cout << "works so far\n";
    return 0;
}

// tests section
/// function to test program
void basic_test()
{
    landscape test_landscape;
    std::vector<agent> test_pop(10);
    test_pop = evolve_pop(test_pop, 10, 10, 10, 1.f, 1.f, test_landscape);

    // check basic output
    assert(static_cast<int>(test_pop.size()) == 10);
    assert(do_main() == 0);
}

/// main function
int main()
{
    // ensure tests run
    basic_test();
    // print output
    return do_main();
}

// end here
