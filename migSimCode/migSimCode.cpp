/// migSimCode.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <cassert>
#include <vector>
#include "landscape.h"
#include "ann.h"
#include "agents.h"
#include "depletion_dynamics.h"
#include <assert.h>
#include <iterator>
//#include "testAgents.cpp"

/// the main function
int do_main()
{
	// run for 100 generations of 100 timesteps
	for (int gen = 0; gen < genmax; gen++)
	{
		std::cout << "gen = " << gen << "\n";
		// increment landscape food on a generation basis
		makeFoodOnLand();

		// loop through timesteps
		for (int t = 0; t < tMax; t++)
		{
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
			for (int ind = 0; ind < popsize; ind++)
			{	
				// resolve chains
				resolveLeaders(ind);
			}

			// handle negative movement
			movePositive();

			// update nAgents on grid cells
			addAgentsToLand();

			// agents get food after competition
			for (int ind = 0; ind < popsize; ind++)
			{
				// get food
				doGetFood(ind);
			}
			
			// reset landscape to remove agents
			resetAgentsOnLand(t);
			
			// output data
			printData(gen, t);
		}

		// udpate landscape with depletion on a generation basis
		depleteLand();

		// print land
		printLand(gen);

		// implement reproduction
		do_reprod();
		//std::cout << "agents reproduce\n";
	}

	return 0;
}

int main()
{
	do_main();

	cout << "works so far\n";
	return 0;
}

// end here
