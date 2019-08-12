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
#include "landscape.h"
#include "agents.h"

using namespace std;

int main()
{
	// init agents
	std::vector<agent> population(popsize);
	// open ofstream of agent positions
	ofstream ofsAgent("testAgentPos.csv");
	for (int iSeason = 0; iSeason < nSeasons; iSeason++)
	{
		cout << "season = " << iSeason << endl;
		// ecological time
		for (int t = 0; t < tMax; t++)
		{
			// print col names
			{
				if (t == 0) { ofsAgent << "gen, id, time, distpeak, energy" << endl; }
			}
			// do forage then do move
			{
				for (int i = 0; i < popsize; i++)
				{
						// get energy
					population[i].doGetFood();
						// do movement
					population[i].doMove();

						// write to file every 10th gen
					if (iSeason % 10 == 0)
					{
						if (t == 0 | t == tMax/2 | t == tMax - 1)
						{
							ofsAgent << iSeason << ", " << i << ", " << t << ", "
							<< abs(population[i].position - currentpeak) << ","
							<< population[i].energy
							<< endl;
						}
					}

				}
			}
			// move peak backwards halfway through generation
			currentpeak += waveVelocity * (t > tMax / 2 ? -1 : 1);
		}

		// SECTION: MAKE NEW GENERATION
		// make fitness vec
		vector<float> fitness_vec;
		float max = 0.f; float min = 0.f;
		for (int a = 0; a < popsize; a++) {

			/*max = max > population[a].energy ? max : population[a].energy;
			min = min < population[a].energy ? min : population[a].energy;*/

			//cout << "fitness " << a << " = " << population[a].energy << endl;
			fitness_vec.push_back(population[a].energy);

			//cout << "fitness vec = "  << fitness_vec[a] << endl;
		}

		// make temp pop vector
		std::vector<agent> pop2(popsize);
		// assign parents
		for (int a = 0; a < popsize; a++) {

			std::discrete_distribution<> weighted_lottery(fitness_vec.begin(), fitness_vec.end());
			int parent_id = weighted_lottery(rng);
			// replicate position and ANN
			pop2[a].position = population[parent_id].position;
			pop2[a].brain = population[parent_id].brain;

			// overwrite energy
			pop2[a].energy = 0.f;

			// mutate ann
			for (auto& w : pop2[a].brain) {
				std::bernoulli_distribution mut_event(0.001); // mutation probability
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
