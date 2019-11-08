#pragma once

// code to init agents

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <unordered_set>
#include <cassert>
#include "ann.h"
#include "params.h"

//// include gsl for distributions
//#include <stdio.h>
//#include <gsl/gsl_rng.h>
//#include <gsl/gsl_randist.h>

using namespace ann;

// spec ann structure
using Ann = Network<float,
	Layer< Neuron<2, activation::rtlu>, 3>, // for now, 2 input for energy cues
	// Layer< Neuron<3, activation::rtlu>, 3>,
	Layer< Neuron<3, activation::rtlu>, 1> // one output, true false
>;

// pick rand node weights
//std::uniform_real_distribution<float> nodeDist(-10.f, 10.f);

#define Pi      3.14159f

// pick rand move angle - uniform distribution over the landscape
std::uniform_real_distribution<float> angleDist(0.f, 360.f);

// bernoulli dist for circlewalk
std::bernoulli_distribution walkDirection(0.5);

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
	agent() : annFollow(0.f), moveAngle(0.f), circWalkDist(1.f),
		circPos(0.f),
		chainLength(0), leader(-1) {};
	~agent() {};
	// agents need a brain, an age, fitness, and movement decision
	Ann annFollow; float moveAngle, circPos, circWalkDist;
	int chainLength, leader;
	// pointer to param
	float* movePointer = &moveAngle; //points to self unless reset
};

/// make vector of agent energy
std::vector<float> agentEnergyVec(popsize, 0.0001f);

/// function to init N agents
std::vector<agent> initAgents(const int& number)
{
	std::vector<agent> population(number);
	return population;
}

/// init agents
std::vector<agent> population = initAgents(popsize);

/// function to list neighbours
// retained in case sensory radius becomes an open topic
std::vector<int> list_neighbours(const int& which_agent)
{
	std::vector<int> currNbrs;
	// use a for loop
	for (int iter = 0; iter < popsize; iter++)
	{
		// collect agent id within sensory range
		//if ((abs(agentPosVec[which_agent] - agentPosVec[iter]) < prange) && which_agent != iter) {
		currNbrs.push_back(iter);
		//}
	}
	// remove self from neighbours
	currNbrs.erase(std::remove(currNbrs.begin(), currNbrs.end(), which_agent), currNbrs.end());
	// shuffle vector
	std::random_shuffle(currNbrs.begin(), currNbrs.end());
	return currNbrs;
}

void resetLeader(std::vector<agent>& population, const int& whichAgent)
{
	// reset leader
	population[whichAgent].leader = -1;
	// // reset moveAnglecopy
	// population[whichAgent].moveAngle = angleDist(rng);
	// // reset param pointer
	// population[whichAgent].movePointer = &population[whichAgent].moveAngle;
}

/// function to entrain to other agent
void chooseLeader(const int& whichAgent, const int& thisNeighbour)
{
	// agents assess neighbour body reserves
	Ann::input_t inputs;
	// get energy cue
	float cueSelf = agentEnergyVec[whichAgent];
	float cueOther = agentEnergyVec[thisNeighbour];

	inputs[0] = static_cast<float> (cueSelf); // debatable function to calc energy
	inputs[1] = static_cast<float> (cueOther); // neighbour energy
	// inputs[1] = energy;
	auto output = population[whichAgent].annFollow(inputs);

	// assign leader if output greater than 0
	population[whichAgent].leader = (output[0] > 0.f ? thisNeighbour : -1);


}

/// function to resolve leadership chains
void resolveLeaders(std::vector<agent> &population, const int& whichAgent)
{
	if (population[whichAgent].leader != -1)
	{
		// constructing leadchains
		std::vector<int> leadchain(1);
		// figure out the first link in the chain
		leadchain[0] = whichAgent;
		// construct the leadership chain
		int iter = population[whichAgent].leader;
		while (population[iter].leader != -1 && leadchain.size() < popsize)
		{
			// add to chain after updating
			leadchain.push_back(iter);

			iter = population[iter].leader;
		}
		// add ultimate leader
		leadchain.push_back(iter);
		// get length of the raw loopy chain
		int initCount = leadchain.size();
		// remove duplicates using set insertion
		int iter2 = 0;
		// temp leadchain
		std::vector<int> templeadchain;
		// make un unordered set to check if duplicates are being added
		std::unordered_set<int> checkDups;

		for (int j = 0; j < initCount && checkDups.find(leadchain[j]) == checkDups.end(); j++)
		{
			templeadchain.push_back(leadchain[j]);
			checkDups.insert(leadchain[j]);
		}

		// set leadchain to reduced size
		leadchain = templeadchain;

		// get new count
		int finalCount = leadchain.size();
		// add chain length - this is the length of the raw loopy chain
		// a value of 200 means a loop was reached
		population[whichAgent].chainLength = finalCount;

		// if leadchain has duplicates, this is a loop
		// resolve by setting everybody in the chain to not move
		if (initCount > finalCount) 
		{

			// breaks the leadership chain at the end
			// has an effect for next leadership chain construction

			population[ leadchain.back() ].leader = -1;
		}

		// check that ultimate leader has no other leader
		int ultLead = leadchain.back();
		assert((population[ultLead].leader) == -1);

		// link forwards along the chain
		for (int iter = 0; iter < leadchain.size() - 1; iter++) {
			// print to check forwards linking
			population[leadchain[iter]].movePointer = &population[leadchain[(iter + 1)]].moveAngle;
		}
		// update backwards along the chain
		for (int l = leadchain.size() - 1; l >= 0; l--)
		{
			population[leadchain[l]].moveAngle = *population[leadchain[l]].movePointer;
		}
	}
	// else {
	// 	population[whichAgent].moveAngleCopy = population[whichAgent].moveAngle;
	// }
}

/// function to convert angle to position
void convertAngleToPos(const int& whichAgent)
{
	float circProp = (sin(population[whichAgent].moveAngle) + 1.f) / 2.f;
	population[whichAgent].circPos = circProp * maxLandPos; 
}

/// convert pos to angle
float convertPosToAngle(const float& thisValue)
{
	float circProp = thisValue / maxLandPos;
	float newAngle = circProp * 360.f;
	return newAngle;
}

/// function to reproduce
void do_reprod()
{
	// make fitness vec
	std::vector<double> fitness_vec;
	float max = 0.f; float min = 0.f;
	for (int a = 0; a < popsize; a++) 
	{
		assert(agentEnergyVec[a] >= 0.f && "agent energy is 0!");

		// std::cout << "fitness " << a << " = " << agentEnergyVec[a] << "\n";
		fitness_vec.push_back(static_cast<double> (agentEnergyVec[a]));
	}

	// make temp pop vector, position and energy vectors
	std::vector<agent> pop2(popsize);
	std::vector<float> agentEnergy2(popsize);
	// assign parents
	for (int a = 0; a < popsize; a++) {

		std::discrete_distribution<> weighted_lottery(fitness_vec.begin(), fitness_vec.end());
		int parent_id = weighted_lottery(rng);
		// replicate ANN
		pop2[a].annFollow = population[parent_id].annFollow;
		// reset who is being followed
		pop2[a].leader = -1;
		// get random movement angle
		pop2[a].moveAngle = angleDist(rng);
		//overwrite movement pointer
		pop2[a].movePointer = &pop2[a].moveAngle;
		// inherit movement param along circle
		pop2[a].circWalkDist = population[parent_id].circWalkDist;
		// reset circular position
		pop2[a].circPos = 0.f;

		// overwrite energy
		agentEnergy2[a] = 0.0001f;

		// mutate ann
		for (auto& w : pop2[a].annFollow) {
			std::bernoulli_distribution mut_event(0.001); // mutation probability
			// probabilistic mutation of ANN
			if (mut_event(rng)) {
				std::cauchy_distribution<double> m_shift(0.0, 0.1); // how much of mutation
				w += static_cast<float> (m_shift(rng));
			}
		}

		// mutate movement parameter
		{
			std::bernoulli_distribution mut_event(0.001); // mutation probability
			// probabilistic mutation of ANN
			if (mut_event(rng))
			{
			std::cauchy_distribution<double> m_shift(0.0, 0.1); // how much of mutation
			pop2[a].circWalkDist += static_cast<float> (m_shift(rng));
			}
		}

	}

	// overwrite old gen - this is more complex in matteo's code
	// no doubt we'll find out why
	population = pop2;

	// now overwrite position and energy vectors
	agentEnergyVec = agentEnergy2;
}

/// function to print data
// func must print gen, id, distance from peak, move param, leader, energy at the end of each generation
void printAgents(const int& gen_p, const int& time_p)
{
	// open or append
	std::ofstream agentofs;
	agentofs.open("dataAgents.csv", std::ofstream::out | std::ofstream::app);

	// col header
	if (gen_p == 0 && time_p == 0) { agentofs << "gen,time,id,movep,circPos,chainlength,leader,energy\n"; }

	// print for each ind
	//if ((gen_p == 0 || gen_p % 5 == 0) && time_p % 20 == 0)
	{
		for (int ind2 = 0; ind2 < popsize; ind2++)
		{
			agentofs
				<< gen_p << ","
				<< time_p << ","
				<< ind2 << ","
				<< population[ind2].moveAngle << ","
				<< population[ind2].circPos << ","
				<< population[ind2].chainLength << ","
				<< population[ind2].leader << ","
				<< agentEnergyVec[ind2] << "\n";
		}
	}
	// close
	agentofs.close();
}

// ends here
