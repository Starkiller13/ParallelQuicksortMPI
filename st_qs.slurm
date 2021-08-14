#!/bin/bash
#SBATCH -J st_quicksort
#SBATCH -o outputs/output_stqs_%j.txt
#SBATCH -e errors/errors_stqs_%j.txt
#SBATCH -t 00:05:00 
#SBATCH --ntasks 1
#SBATCH -c 1
#SBATCH -p allgroups
#SBATCH --mem 12G


srun ./st_quicksort test_files/num1M.txt 1000000

