#pragma once

// code to init agents

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cassert>
#include <cmath>
#include <math.h>
#include "params.hpp"
#include "ann/rndutils.hpp"
#include "ann/rnd.hpp"
#include "ann/ann2.hpp"
#include "landscape.hpp"

using namespace ann;

// spec ann structure
using Ann = Network<float,
Layer< Neuron<2, activation::rtlu>, 3>, // for now, 2 input for energy cues
//    Layer< Neuron<3, activation::rtlu>, 3>,
Layer< Neuron<3, activation::rtlu>, 1> // one output, true false
>;

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

/* define distributions */
// pick random patch
std::uniform_int_distribution<int> position_picker(0, n_patches - 1);

// bernoulli distribution for circlewalk
std::bernoulli_distribution walk_direction(0.5);

// mutation distributions
std::bernoulli_distribution mut_event(0.001); // mutation probability
std::cauchy_distribution<double> m_shift(0.0, 0.01); // how much of mutation

// agent class
class agent
{
public:
    agent() :
        annFollow(0.f), // ann to follow
        pos(0), M(0), D(0.f), // vector position, exploration range, giving up density
        mem_pos(0), // last foraged position
        energy(0.000001f),
        mem_energy(0.f)
    {}
    ~agent() {}

    // agents need a brain, an age, fitness, and movement decision
    Ann annFollow; int pos, M; float D;
    int mem_pos;
    float energy, mem_energy;
    int id_self, id_leader, follow_instances, total_distance;

    bool chooseFollow(const agent& someagent);
    void deplete_and_move(landscape& landscape);
};

/// agent functions here
/// function to shuffle agents for movement order
void shufflePopSeq(std::vector<agent>& vecSomeAgents)
{
    std::shuffle(vecSomeAgents.begin(), vecSomeAgents.end(), rng);
}

/// function to entrain to other agent
bool agent::chooseFollow(const agent& someagent)
{
    // reset leader
    id_leader = -1;
    // agents assess neighbour body reserves
    Ann::input_t inputs;

    inputs[0] = static_cast<float> (energy); // debatable function to calc energy
    inputs[1] = static_cast<float> (someagent.energy); // neighbour energy
    // inputs[1] = energy;
    auto output = annFollow(inputs);

    bool do_follow = output[0] > 0.f;
    if(do_follow) {
        // copy leader foraging site
        pos = someagent.pos;
    }

    return do_follow;
}

/// function to assess remaining agents and shrink move queue
void doFollowDynamic(std::vector<agent>& vecSomeAgents)
{
    assert(vecSomeAgents.size() > 0 && "doFollowDynamic: moveQ is empty at start");

    // make one leader q and one follower q
    std::vector<agent> lead_q;
    std::vector<agent> follow_q = vecSomeAgents;

    int ptl_followers = static_cast<int>(vecSomeAgents.size());
    int ptl_leaders = 0;

    for (int ind = ptl_followers; ind >= 0; ind--) {
        // choose from among leaders if memory of last position is less than D
        if(follow_q[static_cast<size_t>(ind)].mem_energy <
                follow_q[static_cast<size_t>(ind)].D)
        {
            // choose a leader if available
            if(ptl_leaders > 0){
                // first pick a random position
                follow_q[static_cast<size_t>(ind)].pos =
                        position_picker(rnd::reng);

                // change to leader's choice if choose_follow = TRUE
                for (int ld = ptl_leaders; ld >
                     (ptl_leaders - leader_choices) && ld >= (0); ld--) {
                    bool follow_outcome = follow_q[static_cast<size_t>(ind)].chooseFollow(lead_q[static_cast<size_t>(ld)]);
                    if(follow_outcome){
                        // move the agent to the leader q
                        ptl_followers --;
                        ptl_leaders ++;
                        lead_q.push_back(std::move(follow_q[static_cast<size_t>(ind)]));
                        break;
                    }
                }
            }
            else{
                follow_q[static_cast<size_t>(ind)].pos = position_picker(rnd::reng);
            }
        }
    }

    assert(ptl_followers == 0 && "doFollow: followers > 0");
    assert(ptl_leaders == popsize && "doFollow: not all processed");

    // replace population with processed leader queue
    std::swap(vecSomeAgents, lead_q);
    lead_q.clear();
}

/* population level functions */

/// function to reproduce
void do_reprod(std::vector<agent>& pop)
{
    // make fitness vec
    std::vector<double> fitness_vec;
    for (size_t a = 0; static_cast<int>(a) < popsize; a++)
    {
        if (pop[a].energy <= 0.f)
        {
            pop[a].energy = 0.000001f;
        }
        assert(pop[a].energy >= 0.f && "agent energy is 0!");
        fitness_vec.push_back(static_cast<double> (pop[a].energy));
    }

    // weighted lottery
    std::discrete_distribution<> weighted_lottery(fitness_vec.begin(), fitness_vec.end());

    // create new population
    std::vector<agent> tmp_pop(popsize);

    // assign parents
    for (size_t a = 0; static_cast<int>(a) < popsize; a++) {

        size_t parent_id = static_cast<size_t> (weighted_lottery(rnd::reng));

        // replicate ANN
        tmp_pop[a].annFollow = pop[parent_id].annFollow;
        // get random position
        tmp_pop[a].pos = position_picker(rnd::reng);
        // inherit giving up density parameter
        tmp_pop[a].D = pop[parent_id].D;
        // inherit exploration parameter
        tmp_pop[a].M = pop[parent_id].M;

        // mutate ann
        for (auto& w : tmp_pop[a].annFollow) {
            // probabilistic mutation of ANN
            if (mut_event(rng)) {

                w += static_cast<float> (m_shift(rnd::reng));
            }
        }

        // mutate giving up density parameter
        {
            // probabilistic mutation of giving up density
            if (mut_event(rnd::reng))
            {
                tmp_pop[a].D += static_cast<float> (m_shift(rnd::reng));
                if (tmp_pop[a].D > 1.f) {
                    tmp_pop[a].D = 1.f;
                }
                if (tmp_pop[a].D < 0.f) {
                    tmp_pop[a].D = 0.f;
                }
            }
        }
        // mutate exploration parameter
        {
            if (mut_event(rnd::reng))
            {
                tmp_pop[a].M += static_cast<int> (m_shift(rnd::reng));
                if (tmp_pop[a].M < 0) {
                    tmp_pop[a].M = 0;
                }
            }
        }

    }

    // swap tmp pop for pop
    std::swap(pop, tmp_pop);
    tmp_pop.clear();

}

// end here
