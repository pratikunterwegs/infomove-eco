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
	// init landscape
	for (int i = 0; i < landsize; i++)
	{
		landscape[i].resource = pow(peakvalue, -(steepness * (abs(i - initpeak))));
	}

	// init agents
	std::vector<agent> population(popsize);

	//print agent values to check
	/**/
	for (int i = 0; i < popsize; i++)
	{
		/*
		std::cout << "agent " << i << " position: " << population[i].position << " age: "
			<< population[i].age
			<< (population[i].keepGoing == false ? " migrates..." : " stops...");
		std::cout << std::endl;
		*/

		// update site with agents
		population[i].updateSite();
	}

	// open to print landscape
	ofstream ofs ("testLandOutput.csv");

	// open ofstream of agent positions
	ofstream ofsAgent("testAgentPos.csv");

	// ecological time
	for (int t = 0; currentpeak < finalpeak; t++)
	{
		// print col names
		{
			if (t == 0)
			{
				// for landscape
				for (int j = 0; j < landsize; j++)
				{
					ofs << j;
					if (j < landsize - 1)
					{
						ofs << ", ";
					}
				}
				ofs << endl;

				ofsAgent << "agent, time, position, migDist" << endl;
				
			}
		}

		// process agent decisions and outcomes
		{
			// update landscape with presence
			for (int i = 0; i < popsize; i++)
			{
				// update site with agents
				population[i].updateSite();
			}

			// calc fitness
			{
				for (int i = 0; i < popsize; i++)
				{
					population[i].doGetFitness();
				}
			}

			// choose to move or stay
			for (int i = 0; i < popsize; i++)
			{
				population[i].doChoice();
				// print choice
				ofsAgent << i << ", "
					<< t << ", "
					<< population[i].position << ", "
					<< population[i].moveDist << endl;
			}

			// move if chosen to migrate
			for (int i = 0; i < popsize; i++)
			{
				population[i].doMove();
			}
		}



		// update current peak location
		currentpeak += waveVelocity;

		// print to file and update landscape
		for (int i = 0; i < landsize; i++)
		{
			ofs << landscape[i].resource;
			if (i < landsize - 1)
			{
				ofs << ", ";
			}

			landscape[i].resource = pow(peakvalue, -(steepness * (abs(i - currentpeak))));
		}
		ofs << endl;

		// move green wave
		// std::rotate(landscape.begin(), landscape.end() - 1, landscape.end());
	}

	ofs.close();
	ofsAgent.close();

	return 0;
}

//
