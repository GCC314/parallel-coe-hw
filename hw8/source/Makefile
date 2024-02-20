CFLAGS = -std=c++11 -O3 -Wno-unused-result

MPICXX = mpicxx
MPIRUN = mpirun

SERIAL_EXEC_NAME = hw8_serial
MPI_EXEC_NAME = hw8_mpi

MPI_PROCESS_NUM = 8

INPUT_FILE_PATH = ../input.in

MPIRUN_FLAGS = 

.PHONY: clean mpi serial runmpi runserial

mpi:
	$(MPICXX) -D __MPI $(CFLAGS) -o $(MPI_EXEC_NAME) *.cpp

serial:
	$(MPICXX) $(CFLAGS) -o $(SERIAL_EXEC_NAME) *.cpp
	
runmpi: mpi
	$(MPIRUN) -np $(MPI_PROCESS_NUM) $(MPIRUN_FLAGS) $(MPI_EXEC_NAME) $(INPUT_FILE_PATH)

runserial: serial
	./$(SERIAL_EXEC_NAME) $(INPUT_FILE_PATH)

clean:
	- rm $(SERIAL_EXEC_NAME)
	- rm $(MPI_EXEC_NAME)
	- rm ./output/*.log