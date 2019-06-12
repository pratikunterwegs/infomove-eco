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

// dec agent class
class agent
{
	public:
		agent() : brain(0.f), age(0), fitness(0.f), position(0), keepGoing(false) {};
		~agent() {};

		// agents need a brain, an age, fitness, and movement decision
		Ann brain; int age; float fitness; int position; bool keepGoing;

		// functions
		void doChoice();
		void doGetFitness();
		void doAddComp();
		void reproduce();
};

// def choice func
void agent::doChoice()
{
	float in1 = landscape[position].resource;
	float in2 = landscape[position].propAgentsMigrating;

	// agents assess resources and proportion of flock moving forward
	Ann::input_t inputs; inputs[0] = in1; inputs[1] = in2;
	auto output = brain(inputs);

	if (output[0] > 0) { 
		keepGoing = true; 
		landscape[position].nAgentsMigrating += 1;
		landscape[position].propAgentsMigrating += 1.f / static_cast<float> (landscape[position].nAgents);
	}
}

void agent::doAddComp()
{
	landscape[position].totalComp += age;
}

void agent::doGetFitness()
{
	fitness += landscape[position].resource * (static_cast<float> (age) / static_cast<float> (landscape[position].totalComp));
}

// 