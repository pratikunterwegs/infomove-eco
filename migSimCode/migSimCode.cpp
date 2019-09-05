/// migSimCode.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <cassert>
#include <vector>
#include "landscape.h"
#include "ann.h"
#include "agents.h"
#include <assert.h>
#include <iterator>
//#include "testAgents.cpp"

// list of tests

/// test neighbour counting
void test_neighbour_list() 
{
	// create a position vector
	std::vector<float> testPos(5);

	// move last agent (position 4) 100 steps ahead
	testPos[4] = testPos[3] = 100.f;

	// manually get the neighbours of 0 and update
	std::vector<int> neighbours0 = { 1,2 };
	std::vector<int> neighbours4 = { 3 };
	
	// list 0 neighbours using function
	std::vector<int> listNbrs0 = list_neighbours(0, testPos);
	// list 4 neighbours using function
	std::vector<int> listNbrs4 = list_neighbours(4, testPos);

	// assert there are the same neighbours each case - for 0 and 4
	assert(std::equal(neighbours0.begin(), neighbours0.end(), listNbrs0.begin()));
	assert(std::equal(neighbours4.begin(), neighbours4.end(), listNbrs4.begin()));
}

// need to add black box test for the neural network ouput

/// function to print vector output
template <class T>
void out_vec(const std::vector<T>& vec)
{
	std::copy(std::begin(vec), std::end(vec), std::ostream_iterator<T>(std::cout, " "));
}

/// the main function
int do_main()
{
	// run for 100 generations of 100 timesteps
	for (int gen = 0; gen < genmax; gen++)
	{
		cout << "gen = " << gen << "\n";
		// loop through timesteps
		for (int t = 0; t < tMax; t++)
		{
			//cout << "time = " << t << "\n";
			// print current peak
			//std::cout << "currentpeak = " << currentpeak << "\n";

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
				// set follow false manually
				population[ind].follow = false;

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

			// output data
			printData(gen, t);

			// move the resource peak by the wave speed vector
			currentpeak += waveSpeedVec[t];
		}

		// implement reproduction
		do_reprod();

		// rest current peak
		currentpeak = initpeak;
	}

	return 0;
}

///// test do main
//void test_domain()
//{
//	assert(do_main() == 0);
//}

int main()
{
	// run tests
	test_neighbour_list();
	//test_domain();

	do_main();
	

	cout << "works so far\n";
	return 0;
}

// end here
