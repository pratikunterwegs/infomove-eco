/// migSimCode.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <assert.h>
//#include "ann/ann2.hpp"
//#include "ann/rnd.hpp"

// tests section

/// the main function
int do_main()
{
//	initPop(population);
	
//	// run for 100 generations of 100 timesteps
//	for (int gen = 0; gen < genmax; gen++)
//	{
//		std::cout << "gen = " << gen << "\n";
//		// loop through timesteps
//		for (int t = 0; t < tMax; t++)
//		{
//			// shuffle the population, making the first move
//			// queue. no further shuffling in this timestep

//			shufflePopSeq(population);

//			// restting the leaders and implementing follow dynamics
//			for (int ind = 0; ind < popsize; ind++) { population[ind].resetLeader(); }
			
//			doFollowDynamic(population);

//			do_foraging_dynamic(population, foraging_turns);
			
//			//replenish landscape
			
//			// print land
//			//printLand(gen, t);
//		}
//		doMakeFood();
//		// output data
//		print_agent_summary(gen);

//		// implement reproduction
//		do_reprod();
//		//std::cout << "agents reproduce\n";
//	}
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
	// test_doMain();
	   
	// overall do main
	do_main();

	std::cout << "works so far\n";
	return 0;
}

// end here
