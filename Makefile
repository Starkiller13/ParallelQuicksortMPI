CC := gcc
MCC := mpicc
CCFLAGS:= -O2  


default:
	$(CC) $(CCFLAGS) c_files/st_quicksort.c -o st_quicksort
	$(MCC) $(CCFLAGS) c_files/mpi_quicksort.c -o mpi_quicksort

clean:
	rm st_quicksort mpi_quicksort