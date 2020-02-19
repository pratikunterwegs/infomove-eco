#include "agents.hpp"

/// function to print data
// func must print gen, id, distance from peak, move param, leader, energy at the end of each generation
void printAgents(std::vector<agent> population, const int& gen_p, const int& time_p)
{
	// open or append
	std::ofstream agentofs;
	agentofs.open("dataAgents.csv", std::ofstream::out | std::ofstream::app);

	// col header
	if (gen_p == 0 && time_p == 0) { agentofs << "gen,time,id,eeParam,pos,leader,energy\n"; }

	// print for each ind
	if ((gen_p == 0 || gen_p % 1 == 0) && time_p % 5 == 0)
	{
        for (size_t ind2 = 0; static_cast<int>(ind2) < popsize; ind2++)
        {
			agentofs
				<< gen_p << ","
				<< time_p << ","
				<< population[ind2].id_self << ","
				<< population[ind2].tradeOffParam << ","
				<< population[ind2].pos << ","
				<< population[ind2].id_leader << ","
				<< population[ind2].energy << "\n";
		}
	}
	// close
	agentofs.close();
}

/// print agent summary after gen
void print_agent_summary(std::vector<agent> population, const int& gen_p)
{
	// open or append
	std::ofstream agentofs;
	agentofs.open("data_agent_summary.csv", std::ofstream::out | std::ofstream::app);

	// col header
	if (gen_p == 0) { agentofs << "gen,tradeoff,distance,prop_indep,energy\n"; }

	// print for each ind
	if ((gen_p == 0 || gen_p % 1 == 0))
	{
        for (size_t ind2 = 0; static_cast<int>(ind2) < popsize; ind2++)
		{
			agentofs
				<< gen_p << ","
				<< population[ind2].tradeOffParam << ","
				<< population[ind2].total_distance << ","
				<< static_cast<float>(population[ind2].follow_instances) / static_cast<float>(tMax) << ","
				<< population[ind2].energy << "\n";
		}
	}
	// close
	agentofs.close();
}
