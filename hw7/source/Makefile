EXT_ROOT_DIR = lapack-3.11
LIB_DIR = $(EXT_ROOT_DIR)/build
BLAS_HEADER_DIR = $(EXT_ROOT_DIR)/CBLAS/include
LAPACK_HEADER_DIR = $(EXT_ROOT_DIR)/LAPACKE/include

CFLAGS = -std=c++11 -O3
LFLAGS = -llapacke -llapack -lcblas -lrefblas -lm -lgfortran

EXEC_NAME = main

.PHONY: all main lapack clean

main: lapack
	$(CXX) $(CFLAGS) -o $(EXEC_NAME) *.cpp -I$(BLAS_HEADER_DIR) -I$(LAPACK_HEADER_DIR) -L$(LIB_DIR) $(LFLAGS)

all: lapack main
	

lapack:
	- mkdir $(EXT_ROOT_DIR)/build
	$(MAKE) -C $(EXT_ROOT_DIR) blaslib
	$(MAKE) -C $(EXT_ROOT_DIR) cblaslib
	$(MAKE) -C $(EXT_ROOT_DIR) lapacklib
	$(MAKE) -C $(EXT_ROOT_DIR) lapackelib

clean:
	$(MAKE) -C $(EXT_ROOT_DIR) clean
	- rm $(EXEC_NAME)