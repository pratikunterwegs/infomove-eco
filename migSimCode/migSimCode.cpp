// migSimCode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cmath>
#include "landscape.h"



int main()
{
	// init landscape
	for (int i = 0; i < landsize; i++)
	{
		landscape[i] = pow(peakvalue, -(steepness * (abs(i - initpeak))));
	}

	// shift peak with time
	for (int t = 0; t < tMax; t++)
	{
		std::rotate(landscape.begin(), landscape.end() - 1, landscape.end());

		// print to check
		for (int i = 0; i < landsize; i++)
		{
			std::cout << landscape[i] << " ";
		}
		std::cout << std::endl;

	}
}

