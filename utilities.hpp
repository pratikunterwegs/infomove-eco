#ifndef UTILITIES
#define UTILITIES

#include "agents.hpp"
#include <vector>

/// function to print data from an evolved population
void print_agents(std::vector<agent> pop,
                  const float rho, const float phi,
                  const int timesteps, const int turns,
                  const int rep){

    // output filename
    const std::string outfile = "data/agent_" +
            std::to_string(rho).substr(0, 3) + "_" +
            std::to_string(phi).substr(0, 3) + "_" +
            std::to_string(timesteps) + "_" +
            std::to_string(turns)+
            "_rep_" + std::to_string(rep) +
            ".csv";
    std::ofstream agent_ofs;

    // check if exists and write col names
    std::ifstream f(outfile.c_str());
    if(!f.good()){
        agent_ofs.open(outfile, std::ofstream::out);
        agent_ofs << "id,D,M,a,b,c,F\n";
    }

    // append data to existing ofs
    agent_ofs.open(outfile, std::ofstream::out|std::ofstream::app);
    for (size_t i = 0; i < pop.size(); i++) {
        agent_ofs << i << ","
                 << pop[i].D << ","
                 << pop[i].M << ","
                 << pop[i].a << ","
                 << pop[i].b << ","
                 << pop[i].c << ","
                 << pop[i].prop_follow << "\n";
    }

    // write summary with filename to agent data
    // and parameter files
    // start with outfile name
    const std::string summary_out = "data/parameter_summary.csv";
    std::ofstream summary_ofs;

    // if not exists write col names
    std::ifstream f2(summary_out.c_str());
    if(!f2.good()){
        summary_ofs.open(summary_out, std::ofstream::out);
        summary_ofs << "filename,rho,phi,timesteps,turns,rep\n";
    }
    // append if not
    summary_ofs.open(summary_out, std::ofstream::app);
    summary_ofs << outfile << ","
                << rho << ","
                << phi << ","
                << timesteps << ","
                << turns << ","
                << rep << "\n";
}

#endif /* end of include guard: UTILITIES */
