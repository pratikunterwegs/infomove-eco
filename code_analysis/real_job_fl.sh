#! /bin/bash
#SBATCH --job-name=run_infomove_fl_all
#SBATCH --time=05:00:00
#SBATCH --cpus-per-task=1
#SBATCH --mem=9000
#SBATCH --partition=regular

pwd
cd ..

module load R/3.6.1-foss-2018a

module load GCC/8.3.0
module load GSL/2.6-GCC-8.3.0

qmake infomove.pro
make

ml list

Rscript --slave -e 'infomover::run_infomove_fl(type = c("info"), phi = c(5, 25), rho = 0.1, timesteps = 100, a_res = seq(-3, 3, 0.5), b_res = seq(-3, 3, 0.5), M_res = seq(0, 8, 2), leader_choices = c(1,2,5), gradient = 0.5, gradient_m = 2, n_rep = 5)'
