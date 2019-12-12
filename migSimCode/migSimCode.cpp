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

/// the main function
int do_main()
{
	makePositions(landscape);
	initPop(population);
	
	// run for 100 generations of 100 timesteps
	for (int gen = 0; gen < genmax; gen++)
	{
		std::cout << "gen = " << gen << "\n";
		// loop through timesteps
		for (int t = 0; t < tMax; t++)
		{
			// shuffle the population, making the first move
			// queue. no further shuffling in this timestep

			shufflePopSeq(population);

			// restting the leaders and implementing follow dynamics
			for (int ind = 0; ind < popsize; ind++) { population[ind].resetLeader(); }
			
			doFollowDynamic(population);

			// depletion and movement
			// restting the leaders and implementing follow dynamics
			for (int ind = 0; ind < popsize; ind++) { 
				population[ind].doGetFood();
				population[ind].depleteFood();
			}
			
			// output data
			printAgents(gen, t);
			// print land
			printLand(gen, t);
		}
		
		//replenish landscape
		doMakeFood();
		// implement reproduction
		do_reprod();
		//std::cout << "agents reproduce\n";
	}
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
	// all funcs are now removed write new tests
	   
	// overall do main
	do_main();

	std::cout << "works so far\n";
	return 0;
}

// end here
