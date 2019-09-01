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
	agent() : annFollow(0.f), position(0.f), energy(0.f), moveDist(0.f), moveDistCopy(0.f),
		follow(false), leader(0) {};
	~agent() {};

	// agents need a brain, an age, fitness, and movement decision
	Ann annFollow; float energy, position, moveDist, moveDistCopy;
	bool follow;
	int leader;


	std::vector<int> neighbours = {};

	// agent action functions
	void doGetFood();
	// std::vector<int> list_neighbours(int& which_agent, const std::vector<std::vector<float> >& distmatrix);
	void chooseLeader(const int& thisNeighbour);
	void doFollow();
	void doMove();
};

/// function to init N agents
std::vector<agent> initAgents(const int& number)
{
	std::vector<agent> population(number);
	return population;
}

/// init agents
std::vector<agent> population = initAgents(popsize);

/// function to make distance matrix
std::vector<std::vector<float> > make_distmatrix(const std::vector<agent>& population) noexcept
{
	// make holding vector of vectors
	std::vector< std::vector<float> > distmatrix(population.size(), std::vector<float>(population.size()));

	// populate the matrix with pairwise distances
	for (int iter = 0; iter < population.size(); iter++)
	{
		for (int iter2 = 0; iter2 < population.size(); iter2++)
		{
			distmatrix[iter][iter2] = population[iter].position - population[iter2].position;
		}
	}

	return distmatrix;

}

/// function to update distance matrix
void update_distmatrix(std::vector<std::vector<float> >& distmatrix, std::vector<agent>& population)
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
std::vector<int> list_neighbours(const int& which_agent, const std::vector<std::vector<float> >& distmatrix)
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
void agent::chooseLeader(const int& thisNeighbour)
{
	// agents assess neighbour body reserves
	Ann::input_t inputs;
	inputs[0] = pow(peakvalue, -(steepness * (abs(position - currentpeak)))); // debatable
	inputs[1] = static_cast<float> ((population[thisNeighbour]).energy); // neighbour energy
	// inputs[1] = energy;
	auto output = annFollow(inputs);

	// convert float output to bool
	follow = output[0] > 0.f ? true : false;
	
	// assign leader if following engaged
	leader = follow == true? thisNeighbour : 0;
}

/// function to update the moveDistCopy, ie, agents copy leader
// agents revert to inherited move dist if they have no leader
void agent::doFollow()
{
	moveDistCopy = follow ? population[leader].moveDist : moveDist;
}

/// function to move using inherited param or leader param
void agent::doMove()
{
	// add movedistcopy to position
	position += moveDistCopy;
}

/// function to get energy
void agent::doGetFood()
{
	// energy in and divide by neighbours if any
	energy += pow(peakvalue, -(steepness * (abs(position - currentpeak))));
}

/// function to reproduce
void do_reprod()
{
	// make fitness vec
	vector<double> fitness_vec;
	float max = 0.f; float min = 0.f;
	for (int a = 0; a < popsize; a++) {

		/*max = max > population[a].energy ? max : population[a].energy;
		min = min < population[a].energy ? min : population[a].energy;*/

		assert(population[a].energy != 0 && "agent energy is 0!");

		//cout << "fitness " << a << " = " << population[a].energy << endl;
		fitness_vec.push_back(static_cast<double> (population[a].energy));

		//cout << "fitness vec = "  << fitness_vec[a] << endl;
	}

	// make temp pop vector
	std::vector<agent> pop2(popsize);
	// assign parents
	for (int a = 0; a < popsize; a++) {

		std::discrete_distribution<> weighted_lottery(fitness_vec.begin(), fitness_vec.end());
		int parent_id = weighted_lottery(rng);
		// reset next gen position relative to peak
		pop2[a].position = initpeak - (population[parent_id].position - currentpeak);
		// replicate ANN
		pop2[a].annFollow = population[parent_id].annFollow;
		// replicate movement parameters
		pop2[a].moveDist = population[parent_id].moveDist;

		// overwrite energy
		pop2[a].energy = 0.00001f;

		// mutate ann
		for (auto& w : pop2[a].annFollow) {
			std::bernoulli_distribution mut_event(0.01); // mutation probability
			// probabilistic mutation of ANN
			if (mut_event(rng)) {
				std::cauchy_distribution<double> m_shift(0.0, 0.1); // how much of mutation
				w += static_cast<float> (m_shift(rng));
			}
		}

		// mutate movement parameter
		{std::bernoulli_distribution mut_event(0.01); // mutation probability
			// probabilistic mutation of ANN
		if (mut_event(rng))
		{
			std::cauchy_distribution<double> m_shift(0.0, 5.0); // how much of mutation
			pop2[a].moveDist += static_cast<float> (m_shift(rng));
		}
		}

	}

	// overwrite old gen - this is more complex in matteo's code
	// no doubt we'll find out why
	population = pop2;
}

// ends here