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
rm(strategy, m_param, t_max, rep)

# read peregrine password
password = read_lines("private/password.txt")

# connect to cluster and pull from master
s <- ssh_connect("p284074@peregrine.hpc.rug.nl", passwd = password)
ssh_exec_wait(s, command = c("cd infomove/",
                             "cd jobs",
                             "rm *.sh",
                             "cd ..",
                             "git pull",
                             "qmake infomove.pro",
                             "make --silent debug",
                             "make --silent release"))

# send commands
shebang <- readLines("code_r/template_job.sh")
pmap(sim_params, function(strategy, m_param, t_max, rep){
  
  if(!dir.exists("jobs")){
    dir.create("jobs")
  }
  
  shebang[2] <- glue('#SBATCH --job-name=run_pathomove_{strategy}_{m_param}_{t_max}_{rep}')
  {
    command <- glue('./pathomove {strategy} {m_param} {t_max} {rep}')
    jobfile <- glue('jobs/job_pathomove_{strategy}_{m_param}_{t_max}_{rep}.sh')
    
    writeLines(c(shebang, command), con = jobfile)
    scp_upload(s, jobfile, to = "pathomove/")
    file.remove(jobfile)
  }
  
  # run jobs
  ssh_exec_wait(s, command = glue('dos2unix {jobfile}'))
  ssh_exec_wait(s, command = c("cd pathomove",
                               glue('sbatch {jobfile}')))
})

# delete all jobfiles from peregrine and disconnect
ssh_exec_wait(s, command = c("cd pathomove/jobs",
                             "rm *.sh"))
ssh_disconnect(s)

# ends here
