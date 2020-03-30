# script to create infoomove jobs on peregrine

# load libs
library(tidyverse)
library(ssh)
library(glue)

# make dataframe of strategy, m_param, and t_max
# these will be passed as cli args
{phi = as.character(seq(5, 15, 5))
rho = as.character(0.09)
gens = factor("1000")
timesteps = 100
init_d = c(0.1, 0.5, 1.0)
rep = 1:10}

sim_params = crossing(rho, phi, gens, timesteps, init_d, rep)
rm(phi, rho, gens, timesteps, init_d)

# read peregrine password
password = read_lines("private/password.txt")

# connect to cluster and pull from master
s <- ssh_connect("p284074@peregrine.hpc.rug.nl", passwd = password)
ssh_exec_wait(s, command = c("cd infomove/",
                             "cd jobs",
                             "rm *.sh",
                             "cd ..",
                             "git pull",
                             "ml load GCC/8.3.0",
                             "ml load GSL/2.6-GCC-8.3.0",
                             "qmake infomove.pro",
                             "make clean -j4",
                             "make -j4"))

# read job shebang
shebang <- readLines("code_analysis/template_job.sh")

# send commands
pwalk(sim_params, function(rho, phi, gens, timesteps, init_d, rep){

  if(!dir.exists("jobs")){
    dir.create("jobs")
  }

  shebang[2] <- glue('#SBATCH --job-name=run_infomove_phi{phi}_rho{rho}_time{timesteps}_init_d{init_d}_rep{rep}')
  {
    command <- glue('./infomove {phi} {rho} {gens} {timesteps} {init_d} {rep}')
    jobfile <- glue('job_infomove_phi{phi}_rho{rho}_time{timesteps}_init_d{init_d}_rep{rep}.sh')

    writeLines(c(shebang, command), con = glue('jobs/{jobfile}'))
    scp_upload(s, glue('jobs/{jobfile}'), to = "infomove/jobs/")
    file.remove(glue('jobs/{jobfile}'))
  }

  # run jobs; we are in infomove
  ssh_exec_wait(s, command = c("cd infomove/jobs",
                               glue('dos2unix {jobfile}'),
                               glue('sbatch {jobfile}')))
})

# move into jobs, delete scripts and disconnect
ssh_exec_wait(s, command = c("cd infomove/jobs",
                             "rm *.sh",
                             "mv data/*csv ../data",
                             "cd .."))
ssh_disconnect(s)

# ends here
