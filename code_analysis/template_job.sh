#! /bin/bash
#SBATCH --job-name=run_infomove
#SBATCH --time=03:00:00
#SBATCH --cpus-per-task=1
#SBATCH --mem=9000
#SBATCH --partition=regular

pwd
cd ..
module unload GCC/6.3.0
module load GCC/8.3.0
module load GSL/2.6-GCC-8.3.0
ml list
