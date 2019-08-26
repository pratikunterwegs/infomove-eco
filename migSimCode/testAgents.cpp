/// code to run tests on the simulation

// list of tests
/// test that the vector of agents has at least one agent
void test_agent_vec()
{
    // check size of vector
    assert((initAgents(10)).size() == 10);
}

///// test that the distance matrix is correctly created
//void test_make_dmatrix()
//{
//    // make some agents
//    std::vector<agent> testpop = initAgents(10);
//    // check that matrix of 10 agents has 10 * 10 cells
//    std::vector<std::vector<float> > test_dmatrix = make_distmatrix(testpop);
//    assert( test_dmatrix.size() * test_dmatrix[0].size() == 100) );
//
//    // test diagonal is zero
//    for (int iter = 0; iter < testpop.size(); iter++)
//    {
//        assert(test_dmatrix[iter][iter] == 0.f);
//    }
//    
//}
//
///// test the distance matrix is correct updated
//{
//    // create a population
//
//    // make distance matrix
//
//    // move the population manually
//
//    // update the matrix
//
//    // check diagonal remains zero
//
//    // check updated values are correct
//}
//
//
///// test that agents sense the correct number of neighbours
//void test_sense_neighbours()
//{
//    
//
//}
//
///// test that agents share food with correct number of neighbours
//
//
//
///// test that the distance matrix is correctly updated
