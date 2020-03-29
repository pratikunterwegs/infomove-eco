# script to create infoomove jobs on peregrine

# load libs
library(tidyverse)
library(ssh)
library(glue)

# make dataframe of strategy, m_param, and t_max
# these will be passed as cli args
{rho = as.double(seq(0, 5, 0.5))
phi = as.double(1.0)
gens = 1000000
timesteps = 100
turns = 5}

sim_params = crossing(rho, phi, gens, timesteps, turns)
rm(rho, phi, gens, timesteps, turns)

# read peregrine password
password = read_lines("private/password.txt")

# connect to cluster and pull from master
s <- ssh_connect("p284074@peregrine.hpc.rug.nl", passwd = password)
ssh_exec_wait(s, command = c("cd infomove/",
                             "cd jobs",
                             "rm *.sh",
                             "cd ..",
                             "git pull",
                             "make --silent"))

# send commands
shebang <- readLines("code_analysis/template_job.sh")
pmap(sim_params, function(rho, phi, gens, timesteps, turns){
  
  if(!dir.exists("jobs")){
    dir.create("jobs")
  }
  
  shebang[2] <- glue('#SBATCH --job-name=run_infomove_phi{phi}_rho{rho}_time{timesteps}_turns{turns}')
  {
    command <- glue('./infomove {phi} {rho} {gens} {timesteps} {turns}')
    jobfile <- glue('jobs/job_infomove_phi{phi}_rho{rho}_time{timesteps}_turns{turns}.sh')
    
    writeLines(c(shebang, command), con = jobfile)
    scp_upload(s, jobfile, to = "infomove/")
    file.remove(jobfile)
  }
  
  # run jobs; we are in infomove
  ssh_exec_wait(s, command = glue('dos2unix {jobfile}'))
  ssh_exec_wait(s, command = c(glue('sbatch {jobfile}')))
})

# move into jobs, delete scripts and disconnect
ssh_exec_wait(s, command = c("cd jobs",
                             "rm *.sh",
                             "cd .."))
ssh_disconnect(s)

# ends here
