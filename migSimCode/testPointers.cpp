// my first pointer
#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <chrono>
#include <string>
#include <fstream>
#include <ostream>
#include <iterator>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

template <class T>

/// function to cout vector
void cout_vector(const std::vector<T>& vec)
{
  std::copy(std::begin(vec), std::end(vec), std::ostream_iterator<T>(std::cout, " "));
}


/// create class of agent with single attribute
class agent{
public:
    int leader = -1;
    bool move = true;
    int chainLength;
    float param;
    float* paramptr;
};

// decide popsize
const int popsize = 200;

// normal dist of agent id
std::mt19937_64 rng;
std::uniform_int_distribution<int> leaderdist(0, popsize-1);
std::bernoulli_distribution booldist(0.7);

// init vector of agents with param same as position
vector<agent> population(popsize);


int main ()
{

    // section to choose a leader

    // assign ordered parameter and random leader
    for(int i = 0; i < popsize; i++)
    {
        // assigning param
        population[i].param = static_cast<float>(i);
        // assigning leader

        if(booldist(rng)){
            while (population[i].leader == i || population[i].leader == -1) {
                population[i].leader = leaderdist(rng);
            }
        }
        // pointing to leader value
        population[i].paramptr = &population[population[i].leader].param;
    }

    //print leaders and params
    for(int i = 0; i < popsize; i++){
        cout << "agent = " << i << " leader = " << population[i].leader << "\n";
    }

    // section to resolve leadership chains
    // can be done in order
    for(int i = 0; i < popsize; i++)
    {
        if(population[i].leader != -1){

            // constructing leadchains
            std::vector<int> leadchain(1);
            // figure out the first link in the chain
            leadchain[0] = i; leadchain[1] = population[i].leader;

            // construct the leadership chain
            int iter = population[i].leader;
            while (population[iter].leader != -1 && leadchain.size() < popsize) {
                // add to chain after updating
                leadchain.push_back(iter);
                iter = population[iter].leader;

            }
            // get length of the raw loopy chain
            int initCount = leadchain.size();

             // remove duplicates using set insertion
            int iter2 = 0;
             // temp leadchain
            std::vector<int> templeadchain;
            // make un unordered set to check if duplicates are being added
            unordered_set<int> checkDups;

            for(int j = 0; j < leadchain.size() && checkDups.find(leadchain[j]) == checkDups.end(); j++){
                templeadchain.push_back(leadchain[j]);
                checkDups.insert(leadchain[j]);
            }

            // set leadchain to reduced size
            leadchain = templeadchain;
            // get new count
            int finalCount = leadchain.size();


            // add chain length - this is the length of the raw loopy chain
            // a value of 200 means a loop was reached
            population[i].chainLength = finalCount;

            // print final leadchain
            cout << "final leadchain...\n";
            cout_vector(leadchain);
            cout << "\n";

            // if leadchain has duplicates, this is a loop
            // resolve by setting everybody in the chain to not move
            if(initCount > finalCount){
                for (int j=0;j<leadchain.size();j++) {
                    population[leadchain[j]].move = false;
                }
            }


            // link forwards along the chain
            for(int iter = 0; iter < leadchain.size()-1; iter++){
                population[leadchain[iter]].paramptr = &population[leadchain[iter+1]].param;
            }

            // update backwards along the chain
            for(int l = leadchain.size()-1; l >=0; l--){
                population[leadchain[l]].param = * population[leadchain[l]].paramptr;
            }

        }
        else{
            cout << "nothing to be done \n";
        }
    }

    // print inherited params
    for (int i = 0;i < popsize;i++) {
        cout << "agent = " << i
             << " follows = " << population[i].leader
             << " chain = " << population[i].chainLength
             << " updated param = " << population[i].param
             << (population[i].move ? " moves":" stays")
             << "\n";
    }

    return 0;
}
