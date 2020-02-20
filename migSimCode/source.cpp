/// migSimCode.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <fstream>
#include <string>

#include "ann/rnd.hpp"
#include "ann/ann2.hpp"
#include <params.hpp>
#include <landscape.hpp>
#include <agents.hpp>
#include <landscape_dynamics.hpp>
#include <simulations.hpp>
#include <utilities.hpp>

// tests section

/// the main function
int main()
{
    // init new landscape
    landscape landscape;
//  float R = //something from cli args
    std::vector<agent> evolved_pop (popsize);
    float R = 1.f;
    evolved_pop = evolve_pop(genmax, tMax, foraging_turns,
                                                R, landscape);

    // test population on different regimes
    for (size_t r = 0; r < vec_R.size(); r++) {
        std::vector<agent> shifted_pop;
        shifted_pop = regime_shift(evolved_pop, landscape, genmax_shift, tMax,
                                   foraging_turns, vec_R[r]);

        std::cout << "evolved shifted pop: new R =" << vec_R[r] << "\n";
    }

	std::cout << "works so far\n";
	return 0;
}

// end here
