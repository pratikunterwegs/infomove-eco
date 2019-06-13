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

	// update sites with agents
	for (int i = 0; i < popsize; i++)
	{
		/*
		std::cout << "agent " << i << " position: " << population[i].position << " age: "
			<< population[i].age
			<< (population[i].keepGoing == false ? " migrates..." : " stops...");
		std::cout << std::endl;
		*/

		population[i].updateSite();
	}

	// open to print landscape
	ofstream ofs ("testLandOutput.csv");

	// open ofstream of agent positions
	ofstream ofsAgent("testAgentPos.csv");

	for (int iSeason = 0; iSeason < 1; iSeason++)
	{

		// ecological time
		for (int t = 0; t < tMax; t++, currentpeak += waveVelocity)
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

					ofsAgent << "agent, time, position, migDist, energy, fSize" << endl;

				}
			}

			// process agent decisions and outcomes
			{
				// update landscape with presence
				for (int i = 0; i < popsize; i++)
				{
					// remove juveniles based on time dep prob
					population[i].doJuvIndep(t);

					// update site with agents
					population[i].updateSite();
				}

				// calc energy
				{
					for (int i = 0; i < popsize; i++)
					{
						population[i].doGetEnergy();
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
						<< population[i].moveDist << ", "
						<< population[i].energy << ", "
						<< population[i].fSize << endl;
				}

				// move if chosen to migrate
				for (int i = 0; i < popsize; i++)
				{
					population[i].doMove();
				}
			}

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

		}

		// flip landscape for next season
		flipLand(landscape);
	}
	ofs.close();
	ofsAgent.close();

	return 0;
}

//
