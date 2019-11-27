#pragma once
#define _USE_MATH_DEFINES

// code to init agents

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <unordered_set>
#include <cassert>
#include <math.h>
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
std::uniform_real_distribution<float> nodeDist(-10.f, 10.f);

// pick rand move angle - uniform distribution over the landscape
std::uniform_real_distribution<float> angleDist(0.f, 359.f);

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
	agent() : annFollow(nodeDist(rng)), moveAngle(0.f), circWalkDist(1.f),
		circPos(0.f), energy(0.000001f), id_self(),
		chainLength(0), id_leader(-1) {};
	~agent() {};
	// agents need a brain, an age, fitness, and movement decision
	Ann annFollow; float moveAngle, circPos, circWalkDist, energy;
	int chainLength, id_leader, id_self;
	// pointer to param
	float* movePointer = &moveAngle; //points to self unless reset

	void resetLeader();
	void chooseFollow(const agent& someagent);
	void convertAngleToPos();
	void convertPosToAngle();
	void doGetFood();
	void circleWalkAndLearn();
};

/// init agents
std::vector<agent> population(popsize);

/// agent class func to reset leader
void agent::resetLeader()
{
	// reset leader
	id_leader = -1;
}

/// function to shuffle agents for movement order
void shufflePopSeq(std::vector<agent>& vecSomeAgents)
{
	std::shuffle(vecSomeAgents.begin(), vecSomeAgents.end(), rng);
}

/// function to entrain to other agent
void agent::chooseFollow(const agent& someagent)
{
	// agents assess neighbour body reserves
	Ann::input_t inputs;
	// get energy cue
	float cueSelf = energy;
	float cueOther = someagent.energy;

	inputs[0] = static_cast<float> (cueSelf); // debatable function to calc energy
	inputs[1] = static_cast<float> (cueOther); // neighbour energy
	// inputs[1] = energy;
	auto output = annFollow(inputs);

	// assign leader if output greater than 0
	id_leader = (output[0] > 0.f ? someagent.id_self : -1);

}

/// function to assess remaining agents and shrink move queue
// create a temp move queue by shuffling the population
// assign first agent as default moveQ leader
// from second agent allow assessment using chooseFollow()
// if a lead is chosen add to processed q
// update moveQ by removing agents who have chosen a leader
// repeat until moveQ size is 1
// assign procssed q to old q
void doFollowDynamic(std::vector<agent>& vecSomeAgents)
{
	std::vector<agent> tempMoveQ = vecSomeAgents;
	std::vector<agent> processedMoveQ;
	assert(tempMoveQ.size() > 0 && "doFollowDynamic: moveQ is empty at start");
	
	while (tempMoveQ.size() > 1)
	{
		agent moveQLeader = tempMoveQ[0];
		int id_mqleader = moveQLeader.id_self;
		processedMoveQ.push_back(moveQLeader);
		tempMoveQ.erase(tempMoveQ.begin());

		for (int i_moveq = 0; i_moveq < tempMoveQ.size(); i_moveq++)
		{
			tempMoveQ[i_moveq].chooseFollow(moveQLeader);

			if (tempMoveQ[i_moveq].id_leader != -1)
			{
				processedMoveQ.push_back(tempMoveQ[i_moveq]);
			}
		}

		tempMoveQ.erase(std::remove_if(tempMoveQ.begin(), tempMoveQ.end(),
			[](const agent & thisAgent, int id_mqleader) {return(thisAgent.id_leader == id_mqleader); }));
	}

	processedMoveQ.push_back(tempMoveQ[0]);

	assert(processedMoveQ.size() == tempMoveQ.size() && "doFollow: processed queue smaller than input");

	vecSomeAgents = processedMoveQ;

}

/// function to convert angle to position
void agent::convertAngleToPos()
{
	float circProp = (sin(moveAngle) + 1.f) / 2.f;
	assert(circProp <= 1.f && circProp >= 0.f && "func angleToPos: circProp not 0-1");
	circPos = circProp * maxLandPos; 

	assert(circPos <= maxLandPos && "func angleToPos: circ pos calc-ed over land max");
}

/// convert pos to angle
void agent::convertPosToAngle()
{
	float circProp = circPos / maxLandPos;
	assert(circProp <= 1.f && circProp >= 0.f && "func posToAngle: circProp not 0-1");
	float newAngle = circProp * 359.f;
	assert(newAngle <= 359.f && "func posToAngle: angle above 359!");
	moveAngle = newAngle;
}

/// function to reproduce
void do_reprod()
{
	// make fitness vec
	std::vector<double> fitness_vec;
	float max = 0.f; float min = 0.f;
	for (int a = 0; a < popsize; a++) 
	{
		assert(population[a].energy >= 0.f && "agent energy is 0!");

		// std::cout << "fitness " << a << " = " << agentEnergyVec[a] << "\n";
		fitness_vec.push_back(static_cast<double> (population[a].energy));
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
		pop2[a].id_leader = -1;
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
				<< population[ind2].circWalkDist << ","
				<< population[ind2].circPos << ","
				<< population[ind2].chainLength << ","
				<< population[ind2].id_leader << ","
				<< population[ind2].energy << "\n";
		}
	}
	// close
	agentofs.close();
}

// ends here
