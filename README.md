# ParallelQuicksortMPI
***
## Prerequisites:
Needs Open MPI and relative compilers installed

## Usage:
# Local machine:
```
$ make
$ mpirun -np 8 ./mpi_quicksort test_files/numbersBIG.txt 1000000
$ ./st_quicksort test_files/numbersBIG.txt 1000000
```
