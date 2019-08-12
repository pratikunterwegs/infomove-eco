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

//const double meanAge = 5.f;

// spec ann structure
using Ann = Network<float,
	Layer< Neuron<1, activation::rtlu>, 3>, // for now, 1 input for land value
	// Layer< Neuron<3, activation::rtlu>, 3>,
	Layer< Neuron<3, activation::rtlu>, 1> // two outputs, distance and direction
>;

// pick rand node weights
std::uniform_real_distribution<double> dist (-1.f, 1.f);

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

// move cost
//float movecost = 0.01f;

// agent class
class agent
{
	public:
		agent() : brain(static_cast<float>(dist(rng))), position(0.f), energy(0.f) {};
		~agent() {};

		// agents need a brain, an age, fitness, and movement decision
		Ann brain; /*int age;*/ float energy; float position; /* int moveDist; int fSize*/;

		// agent action functions
		void doGetFood();
		void doMove();
};

// input 1 is the landscape value, given by the function peakval^-(steep*(abs(a-peak)))
void agent::doMove()
{
	// agents assess body reserves
	Ann::input_t inputs; 
	inputs[0] = pow(peakvalue, -(steepness * (abs(position - currentpeak)))); 
	// inputs[1] = energy;
	auto output = brain(inputs);

	// process outputs
	position += output[0]; // *(output[1] > 0.f ? 1 : -1); // forwards if greater than 0, else back

	// movement cost
	// energy -= (energy - (output[0] * movecost)) > 0 ? (output[0] * movecost) : 0;

}

void agent::doGetFood()
{
	// energy in is cumulative accuracy
	energy += pow(peakvalue, -(steepness * (abs(position - currentpeak))));
}

// ends here