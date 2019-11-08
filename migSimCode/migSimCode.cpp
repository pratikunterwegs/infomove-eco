/// migSimCode.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <cassert>
#include <vector>
#include "params.h"
#include "ann.h"
#include "agents.h"
#include "landscape_dynamics.h"
#include <assert.h>
#include <iterator>
//#include "testAgents.cpp"

// tests section
/// function to test correct number of agents made
void test_agentMaker()
{
	// make agent vector and check size
	std::vector<agent> test_pop = initAgents(500);
	assert(test_pop.size() == 500 && "wrong popsize\n");
}

/// functison to test correct number of neighbours listed
void test_list_neighbours()
{
	// make agent vector
	std::vector<agent> test_pop = initAgents(500);
	// make neighbours vector
	std::vector<int> test_vec_neighbours = list_neighbours(21);
	// test that agent 21 is not in own neighbours
	assert(std::find(test_vec_neighbours.begin(),
		test_vec_neighbours.end(), 21) == test_vec_neighbours.end()
		&& "agent is own neighbour!");
}

/// function to test leader reset
void test_leaderDynamics()
{
	// make agent vector
	std::vector<agent> test_pop = initAgents(3);
	// set leader manually
	for (int i = 0; i < test_pop.size() - 1; i++)
	{
		test_pop[i].leader = i + 1;
	}
	// resolve leaders
	for (int i = 0; i < test_pop.size(); i++)
	{
		resolveLeaders(test_pop, i);
	}
	// check that moveAnglecopy has been updated
	for (int i = 0; i < test_pop.size() - 1; i++)
	{
		std::cout << "move angle = " << test_pop[i].moveAngle << "\n";
		assert(test_pop[i].moveAngle == test_pop[(i+1)].moveAngle);
	}
	// run reset func
	resetLeader(test_pop, 0);
	// test that leader is now -1
	assert(test_pop[0].leader == -1 && "leader is not reset");
}

/// the main function
int do_main()
{
	makePositions(landscape);
	std::vector<int> agentIdVec;
	for (int i = 0; i < popsize; i++) { agentIdVec.push_back(i); }
	// run for 100 generations of 100 timesteps
	for (int gen = 0; gen < 1; gen++)
	{
		std::cout << "gen = " << 1 << "\n";
		// loop through timesteps
		for (int t = 0; t < tMax; t++)
		{
			// loop through agents and do actions
			for (int ind = 0; ind < popsize; ind++)
			{
				// reset leader, movement etc
				resetLeader(population, ind);
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
				resolveLeaders(population, ind);
				// convert angle to position
				convertAngleToPos(ind);
			}
			// shuffle agent id vector for random order
			std::random_shuffle(agentIdVec.begin(), agentIdVec.end());
			// agents get food and deplete
			for (int ind = 0; ind < popsize; ind++)
			{
				doGetFood(agentIdVec[ind]);
				depleteFood(agentIdVec[ind]);
				circleWalkAndLearn(agentIdVec[ind]);
			}
			
			for (int i =0; i < landPoints; i++)
			{
				std::cout << landscape[i].dFood << " ";
			}
			std::cout << "\n";

			// output data
			printAgents(gen, t);
		}
		// print land
		printLand(gen);
		//replenish landscape
		doMakeFood();
		// implement reproduction
		do_reprod();
		//std::cout << "agents reproduce\n";
	}

	exit(EXIT_FAILURE);
	return 0;
}

/// func to test do main
void test_doMain()
{
	// check do main returns 0
	assert(do_main() == 0 && "errors in the main code");
}

int main()
{
	// run some basic tests
	test_agentMaker();
	test_list_neighbours();
	test_leaderDynamics();
	   
	// overall do main
	do_main();

	std::cout << "works so far\n";
	return 0;
}

// end here
