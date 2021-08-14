CC := gcc
MCC := mpicc
CCFLAGS:= -O2  


default:
	$(CC) $(CCFLAGS) c_files/st_quicksort.c -o st_quicksort
	$(MCC) $(CCFLAGS) c_files/mpi_quicksort.c -o mpi_quicksort

o1:
	$(CC) -O1 c_files/st_quicksort.c -o st_quicksort
	$(MCC) -O1 c_files/mpi_quicksort.c -o mpi_quicksort
o2:
	$(CC) -O2 c_files/st_quicksort.c -o st_quicksort
	$(MCC) -O2 c_files/mpi_quicksort.c -o mpi_quicksort
o3:
	$(CC) -O3 c_files/st_quicksort.c -o st_quicksort
	$(MCC) -O3 c_files/mpi_quicksort.c -o mpi_quicksort

clean:
	rm st_quicksort mpi_quicksort