#pragma once

// code to init agents

#include <iostream>
#include <vector>
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
const float prange = 10.f;

// spec ann structure
using Ann = Network<float,
	Layer< Neuron<2, activation::rtlu>, 3>, // for now, 2 input for land value and agents
	// Layer< Neuron<3, activation::rtlu>, 3>,
	Layer< Neuron<3, activation::rtlu>, 1> // one output, distance
>;

// pick rand node weights
std::uniform_real_distribution<double> dist(-1.f, 1.f);

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
		agent() : annFollow(0.f), position(0.f), energy(0.f), moveDist (0.f), 
			follow (false), leader (0) {};
		~agent() {};

		// agents need a brain, an age, fitness, and movement decision
		Ann annFollow; float energy, position, moveDist;
		bool follow;
		int leader;
		
		
		std::vector<int> neighbours = {};

		// agent action functions
		void doGetFood();
		// std::vector<int> list_neighbours(int& which_agent, const std::vector<std::vector<float> >& distmatrix);
		void chooseFollow(const int &thisNeighbour);
};

/// function to init N agents
std::vector<agent> initAgents(const int &number)
{
	std::vector<agent> population(number);
	return population;
}

/// init agents
std::vector<agent> population = initAgents(popsize);

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
			distmatrix[iter][iter2] = population[iter].position - population[iter2].position;
		}
	}

	return distmatrix;

}

/// function to update distance matrix
void update_distmatrix(std::vector<std::vector<float> > &distmatrix, std::vector<agent> &population)
{
	for (int iter = 0; iter < population.size(); iter++)
	{
		if (population[iter].moveDist > 0.f)
		{
			// some sort of efficient stl based minus tool later
			// consider also the valarray class
			for (int iter2 = 0; iter2 < population.size(); iter2++)
			{
				distmatrix[iter][iter2] = population[iter].position - population[iter2].position;
			}
		}
	}
}

/// function to list neighbours
std::vector<int> list_neighbours(const int & which_agent, const std::vector<std::vector<float> >& distmatrix)
{
	std::vector<int> currNbrs;
	// find neighbours within a sensory radius
	//std::for_each(distmatrix[which_agent].begin(), distmatrix[which_agent].end(), [&](float xy_dist) {if (abs(xy_dist) < prange) n_neighbours++; });
	
	// use a for loop
	for (int iter = 0; iter < distmatrix[which_agent].size(); iter++)
	{
		if (abs(distmatrix[which_agent][iter]) < prange) {
			currNbrs.push_back(iter);
		}
	}

	// update agent neighbours
	return currNbrs;
}

/// function to entrain to other agent
// input 1 is the landscape value, given by the function peakval^-(steep*(abs(a-peak)))
void agent::chooseFollow(const int &thisNeighbour)
{
	// agents assess body reserves
	Ann::input_t inputs; 
	inputs[0] = pow(peakvalue, -(steepness * (abs(position - currentpeak))));
	inputs[1] = static_cast<float> ((population[thisNeighbour]).energy);
	// inputs[1] = energy;
	auto output = annFollow(inputs);

	// convert float output to bool
	follow = output[0] > 0.f ? true : false;

}

/// function to get energy
void agent::doGetFood()
{
	// energy in and divide by neighbours if any
	energy += ((pow(peakvalue, -(steepness * (abs(position - currentpeak))))) / (neighbours > 0 ? static_cast<float> (neighbours): static_cast<float>(1)));
}

// ends here