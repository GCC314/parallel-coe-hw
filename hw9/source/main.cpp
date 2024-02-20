#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <cassert>
#include <complex>

#include <cstdio>

#include "mpi.h"

#include "mat_demo.h"
#include "input.h"
#include "mytimer.h"

#include "calc.h" // separated into a .h

using std::string;
using std::complex;

int main(int argc, char **argv){
    system("test -d output || mkdir output");

    // MPI Initialization
    int rank = 0, size = 1;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Input inp;
    const string input_path = "INPUT.txt"; // specified
    inp.init(input_path);

    assert(inp["matrix_type"] == "file"); // just a check
    assert(inp["calculation"] == "matadd_mpi"); // only support this
    string out_file = inp["output_to_file"];
    bool print_mpi_log = inp["print_mpi_log"] == "1";
    bool timer_print = inp["timer_print"] == "1";
    string matrix_1_path = inp["matrix_1"];
    string matrix_2_path = inp["matrix_2"];
    double alpha = std::stod(inp["alpha"]);
    double beta = std::stod(inp["beta"]);

    mytimer::init();
    // Below are the tasks

    int retcode = [&](){
        // Main task
        // Using lambda to ensure fully destruction of Mat_demo

        int info[2] = {0}; // Store {m, n}

        Mat_Demo<double> M1(0, 0), M2(0, 0);
        // Input matrix

        if(rank == 0){
            M1 = inp.load_matrix(matrix_1_path);
            M2 = inp.load_matrix(matrix_2_path);
            if(!M1.nr() || !M1.nc() || !M2.nr() || !M2.nc()){
                std::cerr << "An empty matrix!\n";
                MPI_Abort(MPI_COMM_WORLD, 1);
                return 1; // control never reaches here
            }
            if(M1.nr() != M2.nr() || M1.nc() != M2.nc()){
                std::cerr << "Inconsistent matrix size!\n";
                MPI_Abort(MPI_COMM_WORLD, 1);
                return 1; // control never reaches here
            }
            info[0] = M1.nr();info[1] = M1.nc();
        } // Input

        MPI_Bcast(&info, 2, MPI_INT, 0, MPI_COMM_WORLD);
        // Broadcast size msg
        
        int m0 = BS(rank, size, info[0]), n0 = info[1];
        Mat_Demo<double> A1(m0, n0), A2(m0, n0); // for Current matrix
        
        if(rank){   // Receive from main process
            mpi_matrecv(0, 1, A1, 0, m0);
            mpi_matrecv(0, 2, A2, 0, m0);
        }else{      // Copy & distribute
            A1.copy(M1, 0);
            A2.copy(M2, 0);
            mpi_distall(size, M1, M2);
        }
        
        mpi_solver(rank, size, alpha, beta, A1, A2); // just add

        if(print_mpi_log) mpi_log_print(rank, size, info[0], info[1], A1);

        if(rank) mpi_dist(0, 0, m0, 3, A1); // send back to main process
        else{   // reduce
            Mat_Demo<double> Ret(info[0], info[1]);
            Ret.copyfrom(A1, 0);
            mpi_reduceall(size, Ret);
            if(out_file != "0")
                mpi_output(out_file, info[0], info[1], Ret);
        }

        return 0;

    }();

    // Above are the tasks

    // print timer infos
    MPI_Barrier(MPI_COMM_WORLD);
    mytimer::end();
    if(timer_print && rank == 0) mytimer::print();

    // End
    MPI_Finalize();
    return retcode;
}