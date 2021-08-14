#!/bin/bash
#SBATCH -J mpi_quicksort
#SBATCH -o output_mpiqs_%j.txt
#SBATCH -e errors_mpiqs_%j.txt
#SBATCH -t 00:02:00
#SBATCH --ntasks 8
#SBATCH -c 1
#SBATCH -p allgroups
#SBATCH --mem 12G

spack load intel-parallel-studio@professional.2019.4 

mpirun -np 8 ./mpi_quicksort test_files/numbersBIG.txt 1000000

