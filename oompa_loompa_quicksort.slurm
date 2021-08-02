#!/bin/bash
#SBATCH -J mpi_quicksort
#SBATCH -o output_mpiqs_%j.txt
#SBATCH -e errors_mpiqs_%j.txt
#SBATCH -t 01:30:00
#SBATCH -n 1
# notice we're using the '-c' option
# to request for OMP threads
#SBATCH -c 8
#SBATCH -p allgroups
#SBATCH --mem 10G

# set this to what you asked with '-c'


export OMP_NUM_THREADS=8

srun ./mpi numbersBIG.txt 1000000

