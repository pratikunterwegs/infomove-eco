# script to create infoomove jobs on peregrine

# load libs
library(tidyverse)
library(ssh)
library(glue)

# read peregrine password
password = read_lines("private/password.txt")

# connect to cluster and pull from master
s <- ssh_connect("p284074@peregrine.hpc.rug.nl", passwd = password)
ssh_exec_wait(s, command = c("cd infomove/",
                             "rm Makefile infomove",
                             "cd jobs",
                             "rm *.sh",
                             "rm *.out",
                             "cd ..",
                             "git pull",
                             "ml load GCC/8.3.0",
                             "ml load GSL/2.6-GCC-8.3.0",
                             "qmake infomove.pro",
                             "make clean -j4",
                             "make -j4"))

# remove old data files
{
  ssh_exec_wait(s, c("cd infomove/data",
  'find . -name "*.csv" -type f -delete'))
}
# read job shebang
shebang <- readLines("code_analysis/template_job.sh")

{
  type = c("noinfo", "info")
  phi = seq(5, 105, 100)
  rho = 0.1
  gens = "100000"
  timesteps = 100
  init_d = c(0.1, 0.5, 1.0)
  replicate = 1:5
}

sim_params <- crossing(type, phi, rho, gens, timesteps, init_d, replicate)

# send commands
pwalk(sim_params, function(type, phi, rho, gens, timesteps, init_d, replicate){

  if(!dir.exists("jobs")){
    dir.create("jobs")
  }

  shebang[2] <- glue('#SBATCH --job-name=run_infomove_type-{type}_phi{phi}_rho{rho}_time{timesteps}_init_d{init_d}_rep{replicate}')
  {
    command <- glue('./infomove {type} {phi} {rho} {gens} {timesteps} {init_d} {replicate}')
    jobfile <- glue('job_infomove_type{type}_phi{phi}_rho{rho}_time{timesteps}_init_d{init_d}_rep{replicate}.sh')

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
                             "mv data/*.csv ../data",
                             "cd .."))
ssh_exec_wait(s, command = c("cd infomove/jobs",
                             "rm *.out",
                             "cd .."))
ssh_disconnect(s)

# ends here
