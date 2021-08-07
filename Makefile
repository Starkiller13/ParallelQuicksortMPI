CC := mpicc
CCFLAGS:= -O2  

TARGET = mpi_quicksort

$(TARGET): $(TARGET).c
	$(CC) $(CCFLAGS) -o $(TARGET) $(TARGET).c
