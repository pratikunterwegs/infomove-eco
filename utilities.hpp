#ifndef UTILITIES
#define UTILITIES

#include "agents.hpp"

/// function to print data from an evolved population
void print_agents(std::string output, std::vector<agent> &pop)
{
	// open or append
	std::ofstream agentofs;
	agentofs.open(output, std::ofstream::out);

	// col header
	// D = giving up density, M = exploration range
	// a,b,c = evolved params for agent assessment
	agentofs << "D,M,a,b,c,F,pos,energy\n"; }
	size_t p_out = static_cast<size_t>(popsize);
	for (size_t i_out = 0; i_out < p_out; i_out++)
	{
		agentofs << pop[i_out].D << ","
			<< pop[i_out].M << ","
			<< pop[i_out].a << "," << pop[i_out].b << "," << pop[i_out].c << ","
			<< pop[i_out].prop_follow << ","
			<< pop[i_out].pos << ","
			<< pop[i_out].energy << "\n";
	}
	agentofs.close();
}

#endif /* end of include guard: UTILITIES */
