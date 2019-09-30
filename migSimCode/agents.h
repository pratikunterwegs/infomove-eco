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

// spec ann structure
using Ann = Network<float,
	Layer< Neuron<2, activation::rtlu>, 3>, // for now, 2 input for land value and agents
	// Layer< Neuron<3, activation::rtlu>, 3>,
	Layer< Neuron<3, activation::rtlu>, 1> // one output, distance
>;

// pick rand node weights
std::uniform_real_distribution<float> nodeDist(-0.1f, 0.1f);

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
	agent() : annFollow(dist(rng)), moveDist(dist(rng)), moveDistCopy(0.f),
		move(true), leader(-1) {};
	~agent() {};

	// agents need a brain, an age, fitness, and movement decision
	Ann annFollow; float moveDist, moveDistCopy;
	bool follow;
	int leader;

	// pointer to param
	float* movePointer;

	// agent action functions
	// void chooseLeader(const int &whichAgent, const int& thisNeighbour);
	void doFollow();
};

/// make vector of agent energy
std::vector<float> agentEnergyVec(popsize, 0.f);

/// function to init N agents
std::vector<agent> initAgents(const int& number)
{
	std::vector<agent> population(number);
	return population;
}

/// init agents
std::vector<agent> population = initAgents(popsize);

/// function to list neighbours
std::vector<int> list_neighbours(const int& which_agent, const std::vector<float> agentPosVec)
{
	std::vector<int> currNbrs;
	
	// use a for loop
	for (int iter = 0; iter < agentPosVec.size(); iter++)
	{
		// collect agent id within sensory range
		if ((abs(agentPosVec[which_agent] - agentPosVec[iter]) < prange) && which_agent != iter) {
			currNbrs.push_back(iter);
		}
	}

	//// remove self from neighbours
	currNbrs.erase(std::remove(currNbrs.begin(), currNbrs.end(), which_agent), currNbrs.end());

	// shuffle vector
	std::random_shuffle(currNbrs.begin(), currNbrs.end());

	return currNbrs;
}

/// function to entrain to other agent
void chooseLeader(const int &whichAgent, const int& thisNeighbour)
{
	// agents assess neighbour body reserves
	Ann::input_t inputs;
	// get energy cue
	float cueSelf = agentEnergyVec[whichAgent];
	float cueOther = agentEnergyVec[thisNeighbour];

	inputs[0] = static_cast<float> (energycue); // debatable function to calc energy
	inputs[1] = static_cast<float> (cueOther); // neighbour energy
	// inputs[1] = energy;
	auto output = annFollow(inputs);

	// convert float output to bool
	population[whichAgent].follow = output[0] > 0.f ? true : false;
	
	// assign leader if following engaged
	population[whichAgent].leader = follow == true? thisNeighbour : -1;

	// link movement pointers if follow is true
	if(population[whichAgent].follow){
	population[whichAgent].movePointer = &population[thisNeighbour].moveDistCopy;
	}

}

/// function to update the moveDistCopy, ie, agents copy leader
// agents revert to inherited move dist if they have no leader
void agent::doFollow()
{
	if (follow)
	{
		movePointer = &population[leader].moveDistCopy;
	}
	else moveDistCopy = moveDist;

}

/// function to get energy
void doGetFood(const int &whichAgent)
{
	// energy in and divide by neighbours if any
	agentEnergyVec[whichAgent] += (1.f / (currentPeak - moveDistCopy));
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

		assert(agentEnergyVec[a] != 0 && "agent energy is 0!");

		//cout << "fitness " << a << " = " << population[a].energy << endl;
		fitness_vec.push_back(static_cast<double> (agentEnergyVec[a]));

		//cout << "fitness vec = "  << fitness_vec[a] << endl;
	}

	// make temp pop vector, position and energy vectors
	std::vector<agent> pop2(popsize);
	std::vector<float> agentPos2(popsize);
	std::vector<float> agentEnergy2(popsize);
	// assign parents
	for (int a = 0; a < popsize; a++) {

		std::discrete_distribution<> weighted_lottery(fitness_vec.begin(), fitness_vec.end());
		int parent_id = weighted_lottery(rng);
		// reset next gen position, everyone starts on the peak
		agentPos2[a] = initpeak;
		// replicate ANN
		pop2[a].annFollow = population[parent_id].annFollow;
		// replicate movement parameters
		pop2[a].moveDist = population[parent_id].moveDist;
		// reset following to false
		pop2[a].follow = false;
		// reset who is being followed
		pop2[a].leader = -1;


		// overwrite energy -  this may be unn
		agentEnergy2[a] = 0.f;

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

	// now overwrite position and energy vectors
	agentPosVec = agentPos2;
	agentEnergyVec = agentEnergy2;
}

/// function to print data
// func must print gen, id, distance from peak, move param, leader, energy at the end of each generation
void printData(const int& gen_p, const int& time_p)
{
	// open or append
	std::ofstream agentofs;
	agentofs.open("dataOut.csv", std::ofstream::out | std::ofstream::app);

	// col header
	if (gen_p == 0 && time_p == 0) { agentofs << "gen,time,id,pos,movep,movepcopy,leader,energy,peakpos\n"; }

	// print for each ind at every 50 generation
	if ((gen_p == 0 || gen_p % 1 == 0) && time_p % 20 == 0) {
		for (int ind2 = 0; ind2 < popsize; ind2++)
		{
			agentofs
				<< gen_p << ","
				<< time_p << ","
				<< ind2 << ","
				<< agentPosVec[ind2] << ","
				<< population[ind2].moveDist << ","
				<< population[ind2].moveDistCopy << ","
				<< population[ind2].leader << ","
				<< agentEnergyVec[ind2] << ","
				<< currentpeak << "\n";
		}
	}
	// close
	agentofs.close();
}

// ends here