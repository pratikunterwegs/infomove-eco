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

const double meanAge = 5.f;

// spec ann structure
using Ann = Network<float,
	Layer< Neuron<2, activation::rtlu>, 3>,
	// Layer< Neuron<3, activation::rtlu>, 3>,
	Layer< Neuron<3, activation::rtlu>, 1>
>;

// pick rand node weights
std::mt19937_64 rng;
std::_Beta_distribution<> dist(1.0, 1.0);

// init with poisson distr age
std::poisson_distribution<int> agePicker (meanAge);

// random value of going on or not
std::bernoulli_distribution migProb (0.5);

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
		agent() : brain(dist(rng)), age(agePicker(rng)), fitness(0.f), position(0), keepGoing(migProb(rng)) {};
		~agent() {};

		// agents need a brain, an age, fitness, and movement decision
		Ann brain; int age; float fitness; int position; bool keepGoing;

		// agent action functions
		void doChoice();
		void doGetFitness();
		void doAge();
		// void reproduce(); // no reprod right now
		void doMove();

		// landscape updating
		void updateSite();
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

	// if output > 0 agents move forward and add to the signal of nAgents migrating
	if (output[0] > 1.f) { 
		keepGoing = true; 
		landscape[position].nAgentsMigrating += 1;
		landscape[position].propAgentsMigrating += 1.f / static_cast<float> (landscape[position].nAgents);
	}
	else
	{
		keepGoing = false;
		landscape[position].nAgentsMigrating -= 1;
		landscape[position].propAgentsMigrating -= 1.f / static_cast<float> (landscape[position].nAgents);
	}
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

// make the movement
void agent::doMove()
{
	// reduce competition on the current position
	landscape[position].totalComp -= age;
	// move
	position++;
}

// update landscape with current position etc
void agent::updateSite()
{
	// add to agents on the landscape
	landscape[position].nAgents++;

	// add to migrating agents
	landscape[position].nAgentsMigrating += keepGoing == true ? 1 : 0;

	// add to competition on the landscape
	landscape[position].totalComp += age;
}

// ends here