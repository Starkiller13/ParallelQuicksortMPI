# ParallelQuicksortMPI

## Prerequisites:
Needs [Open MPI](https://www.open-mpi.org/) and relative compilers installed.
[GNU-gcc](https://gcc.gnu.org/) compiler is also needed in order to compile the sequential algorithm.

## Installation:
```bash
$ git clone https://github.com/Starkiller13/ParallelQuicksortMPI
$ cd ParallelQuicksort
```

## Usage:
### Local machine:
```bash
$ make
$ mpirun -np 4 ./mpi_quicksort test_files/numbersBIG.txt 1000000
$ ./st_quicksort test_files/numbersBIG.txt 1000000
```

### Capri:
```bash
$ spack load intel-parallel-studio@professional.2019.4 
$ make
$ sbatch mpi_qs.slurm
$ sbatch st_qs.slurm
```

## Results:
 Files containing the results of the algorithm applied to the input file can be found respectively in out_mpi.txt and out_st.txt. Verbose and execution times will be displayed in the shell when executing in local; when running on capri there will be a output file in the folder outputs.