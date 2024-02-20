CFLAGS = -std=c++11

MPICXX = mpicxx
MPIRUN = mpirun

MPI_EXEC_NAME = main

MPI_PROCESS_NUM = 2

MPIRUN_FLAGS = 

.PHONY: clean mpi runmpi runmpi_

mpi:
	$(MPICXX) -D __MPI $(CFLAGS) -o $(MPI_EXEC_NAME) *.cpp
	
runmpi: mpi
	$(MPIRUN) -np $(MPI_PROCESS_NUM) $(MPIRUN_FLAGS) ./$(MPI_EXEC_NAME)

runmpi_: # for test
	$(MPIRUN) -np $(MPI_PROCESS_NUM) $(MPIRUN_FLAGS) ./$(MPI_EXEC_NAME)

clean:
	- rm $(MPI_EXEC_NAME)
	- rm ./output/result.txt
	- rm ./output/*.log