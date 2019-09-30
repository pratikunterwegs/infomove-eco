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
//void test_neighbour_list() 
//{
//	// create a position vector
//	std::vector<float> testPos(5);
//
//	// move last agent (position 4) 100 steps ahead
//	testPos[4] = testPos[3] = 100.f;
//
//	// manually get the neighbours of 0 and update
//	std::vector<int> neighbours0 = { 1,2 };
//	std::vector<int> neighbours4 = { 3 };
//	
//	// list 0 neighbours using function
//	std::vector<int> listNbrs0 = list_neighbours(0);
//	// list 4 neighbours using function
//	std::vector<int> listNbrs4 = list_neighbours(4);
//
//	// assert there are the same neighbours each case - for 0 and 4
//	assert(std::equal(neighbours0.begin(), neighbours0.end(), listNbrs0.begin()));
//	assert(std::equal(neighbours4.begin(), neighbours4.end(), listNbrs4.begin()));
//}

// need to add black box test for the neural network ouput

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
				// reset leader, movement etc
				resetLeaderAndMove(ind);

				// return agent neighbours
				std::vector<int> agentNbrs = list_neighbours(ind);

				// choose a leader from among neighbours
				int someNbr = 0;
				while (someNbr < agentNbrs.size() && population[ind].leader == -1) // loop  never entered because leader starts at -1
				{
					chooseLeader(ind, agentNbrs[someNbr]);
					someNbr++;
				}
			}

			// resolve leadership chains at timestep end
			// also get food
			for (int ind = 0; ind < popsize; ind++)
			{	
				// resolve chains
				resolveLeaders(ind);

				// get food
				doGetFood(ind);
			}

			// output data
			printData(gen, t);

			// move the resource peak by the wave speed vector
			//currentpeak += waveSpeedVec[t];
		}

		// implement reproduction
		do_reprod();

		// rest current peak
		//currentpeak = initpeak;
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
	//test_neighbour_list();
	//test_domain();

	do_main();
	

	cout << "works so far\n";
	return 0;
}

// end here
