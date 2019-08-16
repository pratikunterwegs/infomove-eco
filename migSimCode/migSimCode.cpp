// migSimCode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <cassert>
#include <random>
#include <chrono>
#include <string>
#include <algorithm>
#include <functional>
#include "landscape.h"
#include "agents.h"
#include <assert.h>

using namespace std;

int main()
{
	// make distance matrix


	// open ofstream of agent positions from peak
	//ofstream ofsAgent("peakdist.csv");
	ofstream ofsPos("agentpos.csv");

	// write column names
	//ofsAgent << "gen, id, time, distpeak" << endl;
	ofsPos << "gen, id, peakpos, energy, neighbours, pos" << endl;

	// run loop
	for (int igen = 0; igen < nGen; igen++)
	{
		cout << "gen = " << igen << endl;
		// ecological time
		for (int t = 0; t < tMax; t++)
		{
			// sense agents
			for (int i = 0; i < popsize; i++)
			{
				// now sense neighbours
				population[i].doSenseAgent();
			}

			// do forage then do move
			{
				for (int i = 0; i < popsize; i++)
				{
						// get energy
					population[i].doGetFood();
						// do movement
					population[i].doMove();

						if (igen == 0 || ((igen + 1) % 200 == 0))
					{
						ofsPos << igen << "," << i << "," << t << ","
							<< currentpeak << "," << population[i].energy << ","
							<< population[i].neighbours << ","
							<< population[i].position << endl;
					}

				}
			}
			// peak reverses after tmax/2
			currentpeak += waveVelocity; // *(t > tMax / 2 ? -1.f : 1.f);
		}

		// SECTION: MAKE NEW GENERATION
		// make fitness vec
		vector<double> fitness_vec;
		float max = 0.f; float min = 0.f;
		for (int a = 0; a < popsize; a++) {

			/*max = max > population[a].energy ? max : population[a].energy;
			min = min < population[a].energy ? min : population[a].energy;*/

			assert(population[a].energy != 0 && "agent energy is 0!");

			//cout << "fitness " << a << " = " << population[a].energy << endl;
			fitness_vec.push_back(static_cast<double> (population[a].energy));

			//cout << "fitness vec = "  << fitness_vec[a] << endl;
		}

		// make temp pop vector
		std::vector<agent> pop2(popsize);
		// assign parents
		for (int a = 0; a < popsize; a++) {

			std::discrete_distribution<> weighted_lottery(fitness_vec.begin(), fitness_vec.end());
			int parent_id = weighted_lottery(rng);
			// reset next gen position relative to peak
			pop2[a].position = initpeak - (population[parent_id].position - currentpeak);
			// replicate ANN
			pop2[a].brain = population[parent_id].brain;

			// overwrite energy
			pop2[a].energy = 0.00001f;

			// mutate ann
			for (auto& w : pop2[a].brain) {
				std::bernoulli_distribution mut_event(0.01); // mutation probability
				if (mut_event(rng)) {
					std::cauchy_distribution<double> m_shift(0.0, 0.1); // how much of mutation
					w += static_cast<float> (m_shift(rng));
				}
			}
		}

		// overwrite old gen - this is more complex in matteo's code
		// no doubt we'll find out why
		population = pop2;

		// reset current peak
		currentpeak = initpeak;

	}
	ofsAgent.close();
	return 0;
}

// end here
