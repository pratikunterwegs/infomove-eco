#! /bin/bash
#SBATCH --job-name=run_infomove_fl_all
#SBATCH --time=05:00:00
#SBATCH --cpus-per-task=1
#SBATCH --mem=9000
#SBATCH --partition=regular

pwd
cd ..

module load R/3.6.1-foss-2018a

ml list

Rscript --slave -e 'infomover::run_infomove_fl(type = c("info"), phi = c(5, 25), rho = 0.1, timesteps = 100, a_res = seq(-3, 3, 1), b_res = seq(-3, 3, 1), M_res = seq(2, 6, 2), leader_choices = c(1,2,5), gradient = 0.5, gradient_m = 2, n_rep = 5)'
