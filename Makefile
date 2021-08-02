CC := mpiicc
CCFLAGS:= -O2

TARGET = mpi_quicksort

$(TARGET): $(TARGET).c
	$(CC) -o $(TARGET) $(TARGET).c
