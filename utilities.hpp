#ifndef UTILITIES
#define UTILITIES

#include "agents.hpp"
#include <vector>
#include <cstdint>
//#include <filesystem>
#include <chrono>

/// prepare folder structure
/*void prepare_data_folders(std::string type){
    // check if "data/type" exists and create if not
    std::filesystem::file_status s = std::filesystem::file_status{};
    if(!(std::filesystem::status_known(s) ?
            std::filesystem::exists(s) : std::filesystem::exists("data/"+type)))
    {
        std::filesystem::create_directory("data/"+type);
    }
    // then check subfolders
    std::vector<std::string> folders = {"data/"+type+"/agent_summary",
                                       "data/"+type+"/agent_pos",
                                       "data/"+type+"/landscape"};


    for (size_t fol = 0; fol < folders.size(); fol++) {
        if(!(std::filesystem::status_known(s) ?
                std::filesystem::exists(s) : std::filesystem::exists(folders[fol])))
        {
            std::filesystem::create_directory(folders[fol]);
        }
    }
}
*/

/// construct agent output filename
std::vector<std::string> identify_outfile(const std::string type,
                             const int phi, const float rho,
                             const int timesteps,
                             const float init_d,
                             const int leader_choices,
                             const std::string rep){
    // assumes path/type already prepared
    std::string path = "data/" + type;
    // output filename as milliseconds since epoch
    std::string output_id;
    {
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        auto value = now_ms.time_since_epoch();
        // add a random number to be sure of discrete values
        long duration = value.count() +
                static_cast<long>(gsl_rng_uniform_int(r, 10000));

        output_id = std::to_string(duration);
    }

    // write summary with filename to agent data
    // and parameter files
    // start with output id
    const std::string summary_out = path + "/lookup.csv";
    std::ofstream summary_ofs;

    // if not exists write col names
    std::ifstream f2(summary_out.c_str());
    if(!f2.good()){
        summary_ofs.open(summary_out, std::ofstream::out);
        summary_ofs << "filename,type,phi,rho,timesteps,init_d,leader_choices,rep\n";
        summary_ofs.close();
    }
    // append if not
    summary_ofs.open(summary_out, std::ofstream::out | std::ofstream::app);
    summary_ofs << output_id+".csv" << ","
                << type << ","
                << phi << ","
                << rho << ","
                << timesteps << ","
                << init_d << ","
                << leader_choices << ","
                << rep << "\n";
    summary_ofs.close();

    return std::vector<std::string> {path, output_id};
}

/// function to print agent and landscape values
void print_agent_data(std::vector<agent> &pop,
                      const int gen, const int time,
                      std::vector<std::string> output_path)
{
    std::ofstream agent_pos_ofs;
    // check if file is okay
    std::ifstream f(output_path[0] + "/agent_pos/" + output_path[1] + ".csv");
    if(!f.good()){
        agent_pos_ofs.open(output_path[0] + "/agent_pos/" + output_path[1] + ".csv",
                std::ofstream::out);
        agent_pos_ofs << "gen,time,id,pos,pf,energy\n";
        agent_pos_ofs.close();
    }
    for (size_t i = 0; i < pop.size(); i++) {
        agent_pos_ofs.open(output_path[0] + "/agent_pos/" + output_path[1] + ".csv",
                std::ofstream::out | std::ofstream::app);
        agent_pos_ofs << gen << ","
                  << time << ","
                  << i << ","
                  << pop[i].pos << ","
                  << pop[i].prop_follow << ","
                  << pop[i].energy << "\n";
        agent_pos_ofs.close();
    }

}

/// function to print data from an evolved population
void print_agent_summary(std::vector<agent> &pop,
                         const int gen,
                         std::vector<std::string> output_path){

    std::ofstream agent_summary_ofs;
    // check if file is okay
    std::ifstream f(output_path[0] + "/agent_summary/" + output_path[1] + ".csv");
    if(!f.good()){
        agent_summary_ofs.open(output_path[0] + "/agent_summary/" + output_path[1] + ".csv",
                std::ofstream::out);
        agent_summary_ofs << "gen,id,D,M,a,b,pf\n";
        agent_summary_ofs.close();
    }

    for (size_t i = 0; i < pop.size(); i++) {
        agent_summary_ofs.open(output_path[0] + "/agent_summary/" + output_path[1] + ".csv",
                std::ofstream::out | std::ofstream::app);
        agent_summary_ofs << gen << ","
                  << i << ","
                  << pop[i].D << ","
                  << pop[i].M << ","
                  << pop[i].a << ","
                  << pop[i].b << ","
                  << pop[i].prop_follow << "\n";
        agent_summary_ofs.close();
    }
}

#endif /* end of include guard: UTILITIES */
