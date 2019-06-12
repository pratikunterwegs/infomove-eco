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

using namespace std;

int main()
{
	// init landscape
	for (int i = 0; i < landsize; i++)
	{
		landscape[i].resource = pow(peakvalue, -(steepness * (abs(i - initpeak))));
	}

	ofstream ofs ("testLandOutput.csv");
	// check later if open

	// shift peak with time
	for (int t = 0; currentpeak <= finalpeak; t++)
	{
		// print col names
		{
			if (t == 0)
			{
				for (int j = 0; j < landsize; j++)
				{
					ofs << j;
					if (j < landsize - 1)
					{
						ofs << ", ";
					}
				}
				ofs << endl;
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

	return 0;
}

