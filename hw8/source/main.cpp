#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <cassert>

#include "mpi.h"

// We don't use these two in this hw
// #include <cblas.h>
// #include <lapacke.h>


#include "mat_demo.h"
#include "input.h"
#include "mytimer.h"

using std::string;

const int TAG_INFO = 233, TAG_D = 234;

#ifdef __MPI

void mpi_dist(int dst, int l, int m0, Mat_Demo &mat_all){
    // Slice of row [l,r) as tmp
    int n0 = mat_all.nc();
    Mat_Demo tmp(m0, n0);
    for(int x = 0;x < m0;x++)
        for(int y = 0;y < n0;y++)
            tmp(x, y) = mat_all(x + l, y);
    
    // Send info and mat to process dst
    // [bid, m0, n0, sx, sy] and tmp.d
    int info[5] = {dst, m0, n0, l, 0};
    MPI_Send(&info, 5, MPI_INT, dst, TAG_INFO, MPI_COMM_WORLD);
    if(m0 * n0) // if not empty 
        MPI_Send(tmp.d, m0 * n0, MPI_DOUBLE, dst, TAG_D, MPI_COMM_WORLD);
}

void mpi_solver(int rank, int size, Mat_Demo &mat_all, bool print_mpi_log){
    // Distribute matrix
    Mat_Demo mat_slice;
    if(rank == 0){
        int m = mat_all.nr();
        for(int i = 0, l = 0, r = 0;i < size;i++){
            int block_size = m / size + (i < m % size);
            r = l + block_size;
            if(i > 0) mpi_dist(i, l, r - l, mat_all);
            l = r;
        }
    }

    // Receive matrix
    int info[5] = {0, 0, 0, 0, 0};
    if(rank == 0){
        // Directly copy
        int m0 = mat_all.nr() / size + (mat_all.nr() % size > 0);
        int n0 = mat_all.nc();
        mat_slice.resize(m0, mat_all.nc());
        for(int x = 0;x < m0;x++)
            for(int y = 0;y < n0;y++)
                mat_slice(x, y) = mat_all(x, y);
        info[0] = 0;                // rank
        info[1] = m0;info[2] = n0;  // matsize
        info[3] = 0;info[4] = 0;    // startpos
    }else{
        // Recv
        MPI_Status sta;
        MPI_Recv(&info, 5, MPI_INT, 0, TAG_INFO, MPI_COMM_WORLD, &sta);
        mat_slice.resize(info[1], info[2]);
        if(info[1] * info[2]) // if not empty
            MPI_Recv(mat_slice.d, info[1] * info[2], MPI_DOUBLE, 0, TAG_D, MPI_COMM_WORLD, &sta);
    }
    
    // Output to log
    if(!print_mpi_log) return;
    std::string log_fname = "process_" + std::to_string(rank) + ".log";
    std::ofstream ofs("output/" + log_fname);
    ofs << "Process ID: " << rank << "\n";
    ofs << "Block Matrix ID: " << info[0] << "\n";
    ofs << "Block Size: " << info[1] << "x" << info[2] << "\n";
    int sx = info[3], sy = info[4];             // start pos
    int ex = sx + info[1], ey = sy + info[2];   // end pos
    ofs << "Start Position: (" << sx << ", " << sy << ")\n";
    ofs << "End Position: (" << ex << ", " << ey << ")\n";
    ofs << "Block Matrix Elements:\n";
    ofs << mat_slice << "\n";
    ofs.close();
}

#endif

int main(int argc, char **argv){
    if(argc != 2){
        std::cerr << "Usage: " << argv[0] << " [FILE]" << std::endl;
        return 1;
    }
    system("test -d output || mkdir output");

    // MPI Initialization
    int rank = 0, size = 1;
    #ifdef __MPI
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        // std::cout << "This is the parallel ver\n";
        // std::cout << "rank/size=" << rank << "/" << size << "\n";
    #else
        // std::cout << "This is the serial ver\n";
        // Otherwise the only process has rank=0,size=1;
    #endif

    Input inp;
    inp.init(string(argv[1]));

    assert(inp["matrix_type"] == "file"); // just a check

    mytimer::init();
    // Below are the tasks


    // Read matrix in process 0
    Mat_Demo mat_all;
    if(rank == 0) mat_all = inp.load_matrix(inp["matrix_1"]);

    #ifdef __MPI
        // Parallel Mode
        mpi_solver(rank, size, mat_all, inp["print_mpi_log"] == "1");
    #else
        // Serial Mode
        // Directly output to the screen
        if(inp["result_print"] == "1"){
            std::cout << "Matrix size: ";
            std::cout << mat_all.nr() << "x" << mat_all.nc() << "\n"; 
            std::cout << mat_all << std::endl;
        }
    #endif


    // Above are the tasks

    // print timer infos
    #ifdef __MPI
        MPI_Barrier(MPI_COMM_WORLD);
    #endif
    mytimer::end();
    if(inp["timer_print"] == "1")
        if(rank == 0) mytimer::print();

    // End
    #ifdef __MPI
        MPI_Finalize();
    #endif
    return 0;
}