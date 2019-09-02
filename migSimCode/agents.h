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
	agent() : annFollow(dist(rng)), moveDist(dist(rng)), moveDistCopy(0.f),
		follow(false), leader(0) {};
	~agent() {};

	// agents need a brain, an age, fitness, and movement decision
	Ann annFollow; float moveDist, moveDistCopy;
	bool follow;
	int leader;


	std::vector<int> neighbours = {};

	// agent action functions
	void chooseLeader(const int &whichAgent, const int& thisNeighbour);
	void doFollow();
};

/// make vector of agent positions
std::vector<float> agentPosVec(popsize, 0.f);

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
	// find neighbours within a sensory radius
	//std::for_each(distmatrix[which_agent].begin(), distmatrix[which_agent].end(), [&](float xy_dist) {if (abs(xy_dist) < prange) n_neighbours++; });

	// use a for loop
	for (int iter = 0; iter < agentPosVec.size(); iter++)
	{
		// collect agent id within sensory range
		if (abs(agentPosVec[which_agent] - agentPosVec[iter]) < prange) {
			currNbrs.push_back(iter);
		}
	}

	// remove self from neighbours
	currNbrs.erase(std::remove(currNbrs.begin(), currNbrs.end(), which_agent), currNbrs.end());

	// update agent neighbours
	return currNbrs;
}

/// function to entrain to other agent
// input 1 is the landscape value, given by the function peakval^-(steep*(abs(a-peak)))
void agent::chooseLeader(const int &whichAgent, const int& thisNeighbour)
{
	// agents assess neighbour body reserves
	Ann::input_t inputs;
	inputs[0] = static_cast<float> (agentEnergyVec[whichAgent]); // debatable function to calc energy
	inputs[1] = static_cast<float> (agentEnergyVec[thisNeighbour]); // neighbour energy
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
	if (follow)
	{
		moveDistCopy = population[leader].moveDist;
	}
	else moveDistCopy = moveDist;

}

/// function to move using inherited param or leader param
void doMove(const int &whichAgent)
{
	// add movedistcopy to position
	agentPosVec[whichAgent] += population[whichAgent].moveDistCopy;
}

/// function to get energy
void doGetFood(const int &whichAgent)
{
	// energy in and divide by neighbours if any
	agentEnergyVec[whichAgent] += pow(peakvalue, -(steepness * (abs(agentPosVec[whichAgent] - currentpeak))));
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
		// reset next gen position relative to peak
		agentPos2[a] = initpeak - (agentPosVec[parent_id] - currentpeak);
		// replicate ANN
		pop2[a].annFollow = population[parent_id].annFollow;
		// replicate movement parameters
		pop2[a].moveDist = population[parent_id].moveDist;

		// overwrite energy -  this may be unn
		agentEnergy2[a] = 0.00001f;

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
void printData(const int& gen_p)
{
	// open or append
	std::ofstream agentofs;
	agentofs.open("dataOut.csv", std::ofstream::out | std::ofstream::app);

	// col header
	if (gen_p == 0) { agentofs << "gen, id, pos, movep, movepcopy, leader, energy, peakpos\n"; }

	// print for each ind at every 50 generation
	if (gen_p == 0 || gen_p % 50 == 0) {
		for (int ind2 = 0; ind2 < popsize; ind2++)
		{
			agentofs
				<< gen_p << ","
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