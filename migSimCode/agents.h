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
#include "ann.h"
#include "landscape.h"

using namespace std;
using namespace ann;

// move cost
float movecost = 0.01f;

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
std::uniform_real_distribution<double> pos(0.0, 200.0);

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
		agent() : brain(static_cast<float>(dist(rng))), position(static_cast<float> (pos(rng))), energy(0.f) {};
		~agent() {};

		// agents need a brain, an age, fitness, and movement decision
		Ann brain; /*int age;*/ float energy; float position; /* int moveDist; int fSize*/;
		
		int neighbours = 0;

		// agent action functions
		void doGetFood();
		void doMove();
		void doSenseAgent();
};

// init agents vector
std::vector<agent> population(popsize);

// function to sense agents within perception range
void agent::doSenseAgent()
{
	// get perception limits
	float lim1 = position + prange;
	float lim2 = position - prange;
	// run through agents and count those in range
	for (int id = 0; id < popsize; id++)
	{
		neighbours += (population[id].position <= lim1 && population[id].position >= lim2);
	}
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
	energy -= (energy - (output[0] * movecost)) > 0 ? (output[0] * movecost) : 0;

}

void agent::doGetFood()
{
	// energy in 
	energy += (pow(peakvalue, -(steepness * (abs(position - currentpeak))))) / static_cast<float> (neighbours);
}

// ends here