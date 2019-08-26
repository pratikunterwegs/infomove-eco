#pragma once

// code to init agents

#include <iostream>

#include <vector>
#include <random>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <cassert>
#include <random>
#include <chrono>
#include <string>
#include <utility>
#include <algorithm>
#include <functional>
#include <numeric>

#include "ann.h"
#include "landscape.h"

using namespace std;
using namespace ann;

// move cost
// float movecost = 0.01f;

// perception range
float prange = 10.f;

// spec ann structure
using Ann = Network<float,
	Layer< Neuron<2, activation::rtlu>, 3>, // for now, 2 input for land value and agents
	// Layer< Neuron<3, activation::rtlu>, 3>,
	Layer< Neuron<3, activation::rtlu>, 1> // one output, distance
>;

// pick rand node weights
std::uniform_real_distribution<double> dist (-1.f, 1.f);

// pick rand position
// std::uniform_real_distribution<double> pos(0.0, 200.0);

// clear node state
struct flush_rec_nodes
{
	template <typename Neuron, typename T>
	void operator()(T* state, size_t /*layer*/, size_t /*neuron*/)
	{
		auto it = state + Neuron::feedback_scratch_begin;
		for (auto i = 0; i < Neuron::feedback_scratch; ++i) {
			it[i] = T(0);
		}
	}
};

// agent class
class agent
{
	public:
		agent() : brain(0.f), position(0.f), energy(0.f), moveDist (0.f) {};
		~agent() {};

		// agents need a brain, an age, fitness, and movement decision
		Ann brain; float energy, position, moveDist;
		
		int neighbours = 0;

		// agent action functions
		void doGetFood();
		void doMove();
};

/// function to init N agents
std::vector<agent> initAgents(const int &number)
{
	std::vector<agent> population(number);
	return population;
}

/// function to make distance matrix
std::vector<std::vector<float> > make_distmatrix(const std::vector<agent> &population) noexcept
{
	// make holding vector of vectors
	std::vector< std::vector<float> > distmatrix(population.size(), std::vector<float> (population.size()));

	// populate the matrix with pairwise distances
	for(int iter = 0; iter < population.size(); iter++)
	{
		for(int iter2 = 0; iter2 < population.size(); iter2++)
		{
			distmatrix[iter][iter2] = population[iter] - population[iter2];
		}
	}

	return distmatrix;

}

/// function to update distance matrix
void update_distmatrix(std::vector<std::vector<float> > &distmatrix)
{
	for (int iter = 0; iter < population.size(); iter++)
	{
		if (population[iter].moveDist > 0.f)
		{
			// some sort of efficient stl based minus tool later
			// consider also the valarray class
			for (iter2 = 0; iter2 < population.size(); iter2++)
			{
				distmatrix[iter][iter2] = population[iter].position - population[iter2].position;
			}
		}
	}
}

/// function to count neighbours
int count_neighbours(const int &which_agent)
{
	const int n_neighbours = 0;
	// count neighbours within distance of 5
	std::accumulate(distmatrix[which_agent].begin(), distmatrix[which_agent].end(), [&](float xy_dist){if (abs(xy_dist) < 5.f) n_neighbours ++;})

	// output neighbours
	return n_neighbours;
}

// input 1 is the landscape value, given by the function peakval^-(steep*(abs(a-peak)))
void agent::doMove()
{
	// agents assess body reserves
	Ann::input_t inputs; 
	inputs[0] = pow(peakvalue, -(steepness * (abs(position - currentpeak))));
	inputs[1] = static_cast<float> (neighbours);
	// inputs[1] = energy;
	auto output = brain(inputs);

	// process outputs
	position += output[0]; //*(output[1] > 0.f ? 1 : -1); // forwards if greater than 0, else back

	// movement cost
	//energy -= (energy - (output[0] * movecost)) > 0 ? (output[0] * movecost) : 0;

}

void agent::doGetFood()
{
	// energy in and divide by neighbours if any
	energy += ((pow(peakvalue, -(steepness * (abs(position - currentpeak))))) / (neighbours > 0 ? static_cast<float> (neighbours): static_cast<float>(1)));
}

// ends here