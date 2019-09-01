/// migSimCode.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <cassert>
#include <vector>
#include "landscape.h"
#include "ann.h"
#include "agents.h"
#include <assert.h>
//#include "testAgents.cpp"

// list of tests

/// test neighbour counting
void test_neighbour_list() 
{
	// create a position vector
	std::vector<float> testPos(5);

	// move last agent (position 4) 100 steps ahead
	testPos[4] = 100.f;

	// manually get the neighbours of 0 and update
	std::vector<int> neighbours0 = { 1,2,3 };
	
	// list 0 neighbours using function
	std::vector<int> listNbrs0 = list_neighbours(0, testPos);

	// assert there are the same neighbours
	assert(std::equal(neighbours0.begin(), neighbours0.end(), listNbrs0.begin()));
	
	// all other should have 3 neighbours (4 minus self)
	for (int iter = 0; iter < testPos.size() - 1; iter++)
	{
		assert( (list_neighbours(iter, testPos)).size() == 3);
	}
}

// need to add black box test for the neural network ouput

/// the main function
int do_main()
{
	// run for 100 generations of 100 timesteps
	for (int gen = 0; gen < 10; gen++)
	{
		cout << "gen = " << gen << "\n";
		// loop through timesteps
		for (int t = 0; t < tMax; t++)
		{
			cout << "time = " << t << "\n";
			// print current peak
			std::cout << "currentpeak = " << currentpeak << "\n";

			// loop through agents and do actions
			for (int ind = 0; ind < popsize; ind++)
			{
				/// section to forage
				// update the energy vector
				doGetFood(ind);

				/// section to choose a leader and follow (copy movedist)
				// return agent neighbours
				std::vector<int> agentNbrs = list_neighbours(ind, agentPosVec);

				// choose a leader and follow
				int nbr = 0;
				// while there is no leader and all neighbours have not been sampled
				while (population[ind].follow == false && nbr < agentNbrs.size())
				{
					// choose a leader if any and include agent
					population[ind].chooseLeader(ind, agentNbrs[nbr]);
					nbr++;
				}
				population[ind].doFollow();

				/// section to move
				// update the position vector
				doMove(ind);
				
			}

			// move the resource peak by the wave speed
			currentpeak += waveVelocity;

			// print current peak
			std::cout << "currentpeak = " << currentpeak << "\n";
		}

		// rest current peak
		currentpeak = initpeak;
	}

	return 0;
}

int main()
{
	// run tests
	test_neighbour_list();

	cout << "works so far\n";
	return 0;
}

// end here
