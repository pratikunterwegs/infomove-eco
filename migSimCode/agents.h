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

// spec ann structure
using Ann = Network<float,
	Layer< Neuron<2, activation::rtlu>, 3>,
	Layer< Neuron<3, activation::rtlu>, 3>,
	Layer< Neuron<3, activation::rtlu >, 1>
>;

// pick rand node weights
std::mt19937_64 rng;
std::_Beta_distribution<> dist(2.0, 2.0);

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

// dec agent class
class agent
{
	public:
		agent() : brain(dist(rng)), age(0), fitness(0.f), position(0), keepGoing(false) {};
		~agent() {};

		// agents need a brain, an age, fitness, and movement decision
		Ann brain; int age; float fitness; int position; bool keepGoing;

		// functions
		void doChoice();
		void doGetFitness();
		void doAddComp();
		void doAge();
		// void reproduce(); // no reprod right now
};

// def choice func
// agents have 2 inputs and 1 output
void agent::doChoice()
{
	float in1 = landscape[position].resource;  // input 1 is resource level
	float in2 = landscape[position].propAgentsMigrating;  // input 2 is nmber of other agents moving forward

	// agents assess resources and proportion of flock moving forward
	Ann::input_t inputs; inputs[0] = in1; inputs[1] = in2;
	auto output = brain(inputs);

	// if output > 0 agents move forward and add to the signal
	if (output[0] > 0) { 
		keepGoing = true; 
		landscape[position].nAgentsMigrating += 1;
		landscape[position].propAgentsMigrating += 1.f / static_cast<float> (landscape[position].nAgents);
	}
}

// agents add competitive pressure proportional to age
// this can be increased in mechanistic detail
void agent::doAddComp()
{
	landscape[position].totalComp += age;
}

// agents extract resources scaled by their age-based competitiveness on the site
void agent::doGetFitness()
{
	fitness += landscape[position].resource * (static_cast<float> (age) / static_cast<float> (landscape[position].totalComp));
}

// function agents age, maybe be useful later
void agent::doAge()
{
	age++;
}

// ends here