#ifndef UTILITIES
#define UTILITIES

#include "agents.hpp"
#include <vector>

/// function to print data from an evolved population
void print_agents(std::string output, std::vector<agent> pop)
{
	// open or append
	std::ofstream agentofs;
	agentofs.open(output, std::ofstream::out);

	// col header
	// D = giving up density, M = exploration range
	// a,b,c = evolved params for agent assessment
    agentofs << "D,M,a,b,c,F,pos,energy\n";
    for (size_t iout = 0; iout < pop.size(); iout++)
	{
		agentofs << pop[iout].D << ","
			<< pop[iout].M << ","
			<< pop[iout].a << "," << pop[iout].b << "," << pop[iout].c << ","
			<< pop[iout].prop_follow << ","
			<< pop[iout].pos << ","
			<< pop[iout].energy << "\n";
	}
	agentofs.close();
}

#endif /* end of include guard: UTILITIES */
