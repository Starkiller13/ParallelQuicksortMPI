# ParallelQuicksortMPI

## Prerequisites:
Needs [Open MPI](https://www.open-mpi.org/) and relative compilers installed.
[GNU-gcc](https://gcc.gnu.org/) compiler is also needed in order to compile the sequential algorithm.

## Installation:
```bash
$ git clone https://github.com/Starkiller13/ParallelQuicksortMPI
```

## Usage:
### Local machine:
```bash
$ make
$ mpirun -np 8 ./mpi_quicksort test_files/numbersBIG.txt 1000000
$ ./st_quicksort test_files/numbersBIG.txt 1000000
```

### Capri:
```bash
$ spack load intel-parallel-studio@professional.2019.4 
$ make
$ sbatch mpi_qs.slurm
$ sbatch st_qs.slurm
```
