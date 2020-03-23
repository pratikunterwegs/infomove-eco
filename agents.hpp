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
#include "landscape.hpp"

/* define distributions */
// pick random patch
std::uniform_int_distribution<int> position_picker(0, n_patches - 1);

// normal distribution for follow prob
std::normal_distribution<float> follow_prob_picker(0.5f, 0.2f);

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
        // heritable params for interaction follow botero et al. 2010
        a(0.f), // inflection point
        b(0.f), // slope for own quality
        c(0.f), // slope for leader quality
        pos(0), M(0), D(0.f), // vector position, exploration range, giving up density
        mem_pos(0), // last foraged position
        energy(0.000001f),
        mem_energy(0.f),
        prop_follow(0.f)
    {}
    ~agent() {}

    // agents need a brain, an age, fitness, and movement decision
    float a, b, c;
    int pos, M; float D;
    int mem_pos;
    float energy, mem_energy, prop_follow;
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
    // agents assess neighbour body reserves

    float p_follow = 1 / (1 + (exp(a - (b*energy) - (c*someagent.energy))));

    bool do_follow = p_follow > follow_prob_picker(rng);
    if(do_follow) {
        // copy leader foraging site
        pos = someagent.pos;
        prop_follow = (prop_follow + 1.f)/2.f;
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

    int ind = ptl_followers - 1;

    while(ptl_followers > 0) {

        // choose from among leaders if memory of last position is less than D
        if(follow_q[static_cast<size_t>(ind)].mem_energy <
                follow_q[static_cast<size_t>(ind)].D)
        {
            // first pick a random position
            follow_q[static_cast<size_t>(ind)].pos =
                    position_picker(rng);

            bool follow_outcome = false;
            int ld = ptl_leaders - 1;

            // choose a leader if available
            if(ptl_leaders == 0){

                lead_q.push_back(std::move(follow_q[static_cast<size_t>(ind)]));
                follow_q.pop_back();
            }

            else if(ptl_leaders < leader_choices){
                while (ld >= 0 && follow_outcome == false) {
                    follow_outcome = follow_q[static_cast<size_t>(ind)].chooseFollow(lead_q[static_cast<size_t>(ld)]);
                    // move indiv from follow to lead
                    if(follow_outcome){

                        lead_q.push_back(std::move(follow_q[static_cast<size_t>(ind)]));
                        follow_q.pop_back();
                    }
                    ld --;
                }
            }

            else {

                while(follow_outcome == false && ld > ptl_leaders - leader_choices){
                    follow_outcome = follow_q[static_cast<size_t>(ind)].chooseFollow(lead_q[static_cast<size_t>(ld)]);

                    if(follow_outcome){

                        lead_q.push_back(std::move(follow_q[static_cast<size_t>(ind)]));
                        follow_q.pop_back();
                    }
                    ld --;
                }
            }
        }
        else {
            lead_q.push_back(std::move(follow_q[static_cast<size_t>(ind)]));
            follow_q.pop_back();
        }

        ptl_followers --;
        ptl_leaders ++;

//        std::cout << "leaders = " << ptl_leaders << "\n";
    }
    assert(lead_q.size() == vecSomeAgents.size() && "agents lost from q");

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
    for (size_t ind_2 = 0; static_cast<int>(ind_2) < popsize; ind_2++)
    {
        if (pop[ind_2].energy <= 0.f)
        {
            pop[ind_2].energy = 0.000001f;
        }
        assert(pop[ind_2].energy >= 0.f && "agent energy is 0!");
        fitness_vec.push_back(static_cast<double> (pop[ind_2].energy));
    }

    // weighted lottery
    std::discrete_distribution<> weighted_lottery(fitness_vec.begin(), fitness_vec.end());

    // create new population
    std::vector<agent> tmp_pop(popsize);

    // assign parents
    for (size_t ind_2 = 0; static_cast<int>(ind_2) < popsize; ind_2++) {

        size_t parent_id = static_cast<size_t> (weighted_lottery(rng));

        // replicate gene loci controlling following
        tmp_pop[ind_2].a = pop[parent_id].a;
        tmp_pop[ind_2].b = pop[parent_id].b;
        tmp_pop[ind_2].c = pop[parent_id].c;
        // get random position
        tmp_pop[ind_2].pos = position_picker(rng);
        // inherit giving up density parameter
        tmp_pop[ind_2].D = pop[parent_id].D;
        // inherit exploration parameter
        tmp_pop[ind_2].M = pop[parent_id].M;

        // mutate giving up density parameter
        {
            // probabilistic mutation of giving up density
            if (mut_event(rng))
            {
                tmp_pop[ind_2].D += static_cast<float> (m_shift(rng));
                if (tmp_pop[ind_2].D > 1.f) {
                    tmp_pop[ind_2].D = 1.f;
                }
                if (tmp_pop[ind_2].D < 0.f) {
                    tmp_pop[ind_2].D = 0.f;
                }
            }
        }
        // mutate exploration parameter
        {
            if (mut_event(rng))
            {
                tmp_pop[ind_2].M += static_cast<int> (m_shift(rng));
                if (tmp_pop[ind_2].M < 0) {
                    tmp_pop[ind_2].M = 0;
                }
            }
        }
        // mutate a
        {
            if (mut_event(rng))
            {
                tmp_pop[ind_2].a += static_cast<int> (m_shift(rng));
                if (tmp_pop[ind_2].a < 0.f) {
                    tmp_pop[ind_2].a = 0.f;
                }
                if (tmp_pop[ind_2].a > 1.f) {
                    tmp_pop[ind_2].a = 1.f;
                }
            }
        }
        // mutate b
        {
            if (mut_event(rng))
            {
                tmp_pop[ind_2].b += static_cast<int> (m_shift(rng));
                if (tmp_pop[ind_2].b < 0.f) {
                    tmp_pop[ind_2].b = 0.f;
                }
                if (tmp_pop[ind_2].b > 1.f) {
                    tmp_pop[ind_2].b = 1.f;
                }
            }
        }
        // mutate c
        {
            if (mut_event(rng))
            {
                tmp_pop[ind_2].c += static_cast<int> (m_shift(rng));
                if (tmp_pop[ind_2].c < 0.f) {
                    tmp_pop[ind_2].c = 0.f;
                }
                if (tmp_pop[ind_2].c > 1.f) {
                    tmp_pop[ind_2].c = 1.f;
                }
            }
        }

    }

    // swap tmp pop for pop
    std::swap(pop, tmp_pop);
    tmp_pop.clear();

}

// end here
