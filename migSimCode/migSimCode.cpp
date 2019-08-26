/// migSimCode.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <cassert>
#include <vector>
#include "landscape.h"
#include "ann.h"
#include "agents.h"
#include <assert.h>
//#include "testAgents.cpp"

// list of tests
/// test that the vector of agents has at least one agent
void test_agent_vec()
{
	// check size of vector
	assert((initAgents(10)).size() == 10);
}

/// test that the distance matrix is correctly created
void test_make_dmatrix()
{
    // make some agents
    std::vector<agent> testpop = initAgents(10);
    // check that matrix of 10 agents has 10 * 10 cells
    std::vector<std::vector<float> > test_dmatrix = make_distmatrix(testpop);
    assert( (test_dmatrix.size() * test_dmatrix[0].size()) == 100);

    // test diagonal is zero
    for (int iter = 0; iter < testpop.size(); iter++)
    {
        assert(test_dmatrix[iter][iter] == 0.f);
    }
    
}

/// test the distance matrix is correct updated
void test_distmatrix_update()
{
    // create a population
	std::vector<agent> testpop = initAgents(25);
    // make distance matrix
	std::vector<std::vector<float> > test_dmatrix = make_distmatrix(testpop);

	// move the population manually per the position
	for (int someiter = 0; someiter < testpop.size(); someiter++)
	{
		testpop[someiter].position = static_cast<float>(someiter);
		testpop[someiter].moveDist = static_cast<float>(someiter) + 0.1f;
	}
	
    // update the matrix
	update_distmatrix(test_dmatrix, testpop);

    // check diagonal remains zero
	for (int someiter=0; someiter < testpop.size(); someiter++)
	{
		assert(test_dmatrix[someiter][someiter] == 0.f);
	}
	
    // check agent i is 1 steps from agent i-1
	for (int someiter = 0; someiter < test_dmatrix.size(); someiter++)
	{
		assert(static_cast<int>(test_dmatrix[someiter][0]) == someiter);
		
	}
}


int main()
{
	std::vector<agent> initpop = initAgents(popsize);

	// run tests
	test_agent_vec();
	test_make_dmatrix();
	test_distmatrix_update();

	// print dist matrix


	return 0;
}

// end here
