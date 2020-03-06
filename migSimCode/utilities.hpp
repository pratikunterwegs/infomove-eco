#include "agents.hpp"

/// function to print data
// func must print gen, id, distance from peak, move param, leader, energy at the end of each generation
void printAgents(std::vector<agent> population, const int& gen_p, const int& time_p)
{
	// open or append
	std::ofstream agentofs;
	agentofs.open("dataAgents.csv", std::ofstream::out | std::ofstream::app);

	// col header
    if (gen_p == 0 && time_p == 0) { agentofs << "gen,time,D,M,pos,energy\n"; }

	// print for each ind
	if ((gen_p == 0 || gen_p % 1 == 0) && time_p % 5 == 0)
	{
        for (size_t ind2 = 0; static_cast<int>(ind2) < popsize; ind2++)
        {
			agentofs
				<< gen_p << ","
				<< time_p << ","
                << population[ind2].D << ","
                << population[ind2].M << ","
                << population[ind2].pos << ","
				<< population[ind2].energy << "\n";
		}
	}
	// close
	agentofs.close();
}

/// function to print landscape values
//void printLand(const int& gen_p, const int& t_p)
//{
//	// open or append
//	std::ofstream landofs;
//	landofs.open("dataLand.csv", std::ofstream::out | std::ofstream::app);
//	// col header
//	if ((gen_p == 0) && (t_p == 0)) { landofs << "gen,t,pos,food\n"; }
//	// print for each land cell
//	{
//		for (int landcell = 0; landcell < landscape.size(); landcell++)
//		{
//			landofs
//				<< gen_p << ","
//				<< t_p << ","
//				<< landcell << ","
//				<< landscape[landcell].dFood << "\n";
//		}
//		//close
//		landofs.close();
//	}
//}

// ends here
