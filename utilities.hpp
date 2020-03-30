#ifndef UTILITIES
#define UTILITIES

#include "agents.hpp"
#include <vector>

/// construct agent output filename
std::string identify_outfile(const int phi, const float rho,
                        const int timesteps,
                        const float init_d,
                        const int rep){
    // output filename
    const std::string outfile = "data/agent_" +
            std::to_string(phi) + "_" +
            std::to_string(rho).substr(0, 3) + "_" +
            std::to_string(timesteps) + "_" +
            std::to_string(init_d).substr(0,3)+
            "_rep_" + std::to_string(rep) +
            ".csv";


    // write summary with filename to agent data
    // and parameter files
    // start with outfile name
    const std::string summary_out = "data/parameter_summary.csv";
    std::ofstream summary_ofs;

    // if not exists write col names
    std::ifstream f2(summary_out.c_str());
    if(!f2.good()){
        summary_ofs.open(summary_out, std::ofstream::out);
        summary_ofs << "filename,phi,rho,timesteps,init_d,rep\n";
        summary_ofs.close();
    }
    // append if not
    summary_ofs.open(summary_out, std::ofstream::out | std::ofstream::app);
    summary_ofs << outfile << ","
                << phi << ","
                << rho << ","
                << timesteps << ","
                << init_d << ","
                << rep << "\n";
    summary_ofs.close();

    return outfile;
}

/// function to print data from an evolved population
std::string print_agents(std::vector<agent> pop,
                  const int gen, std::string outfile){

    std::ofstream agent_ofs;
    // append data to existing ofs

    // check if file is okay
    std::ifstream f(outfile);
    if(!f.good()){
        agent_ofs.open(outfile, std::ofstream::out);
        agent_ofs << "gen,id,D,M,a,b,F\n";
        agent_ofs.close();
    }

    for (size_t i = 0; i < pop.size(); i++) {
        agent_ofs.open(outfile, std::ofstream::out | std::ofstream::app);
        agent_ofs << gen << ","
                  << i << ","
                  << pop[i].D << ","
                  << pop[i].M << ","
                  << pop[i].a << ","
                  << pop[i].b << ","
                  << pop[i].prop_follow << "\n";
        agent_ofs.close();
    }

    return outfile;
}

#endif /* end of include guard: UTILITIES */
