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

// tests section

/// the main function
int main()
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

// end here
