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

using std::string;
using std::complex;

const int TAGS[] = {0, 628, 629, 630}; // tags for mpi communication

inline int L(int rank, int size, int m){ // Leftpos of each block
    return rank * (m / size) + std::min(rank, m % size);
}

inline int BS(int rank, int size, int m){ // width of each block
    return m / size + (rank < m % size);
}

// distribute matrix from M to dst
// offset = (l, 0),
// length of dim1 = m0
void mpi_dist(int dst, int l, int m0, int tag, Mat_Demo<double>& M){
    int n0 = M.nc();
    if(!(m0 * n0)) return;
    MPI_Send(M.d + l * n0, m0 * n0, MPI_DOUBLE, dst, TAGS[tag], MPI_COMM_WORLD);
}

// split and distribute M1 & M2 
void mpi_distall(int size, Mat_Demo<double>& M1, Mat_Demo<double>& M2){
    int m = M1.nr(), n = M1.nc();
    for(int i = 0, l = 0, r = 0;i < size;i++){
        int bsize = BS(i, size, m);
        r = l + bsize;
        if(i > 0){
            mpi_dist(i, l, bsize, 1, M1);
            mpi_dist(i, l, bsize, 2, M2);
        }
        l = r;
    }
}

// receive matrix from src to M
// offset = (l, 0),
// length of dim1 = m0
void mpi_matrecv(int src, int tag, \
    Mat_Demo<double> &M, int l, int m0){
    int n0 = M.nc();
    if(!(m0 * n0)) return;
    MPI_Status sta;
    MPI_Recv(M.d + l * n0, m0 * n0, MPI_DOUBLE, src, TAGS[tag], MPI_COMM_WORLD, &sta);
}

// receive and reduce to a result matrix M
void mpi_reduceall(int size, Mat_Demo<double>& M){
    int m = M.nr(), n = M.nc();
    for(int i = 0, l = 0, r = 0;i < size;i++){
        int bsize = BS(i, size, m);
        r = l + bsize;
        if(i > 0) mpi_matrecv(i, 3, M, l, bsize);
        l = r;
    }
}

// mult and add
void mpi_solver(int rank, int size, double alpha, double beta, \
    Mat_Demo<double> &A1, Mat_Demo<double> &A2){
    int m0 = A1.nr(), n0 = A1.nc();
    for(int x = 0;x < m0;x++) for(int y = 0;y < n0;y++)
        A1(x, y) = alpha * A1.get(x, y) + beta * A2.get(x, y);
}

// dump to log
void mpi_log_print(int rank, int size, int m, int n, \
    const Mat_Demo<double> &M){
    string log_fname = "process_" + std::to_string(rank) + ".log";
    std::ofstream ofs("output/" + log_fname);
    if(!ofs.is_open()){
        std::cerr << "Failed to open file \n";
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    ofs << "Process ID: " << rank << "\n";
    ofs << "Block Matrix ID: " << rank << "\n";
    int m0 = BS(rank, size, m), n0 = n;
    ofs << "Block Size: " << m0 << "x" << n0 << "\n";
    int sx = L(rank, size, m);
    int sy = 0; // start pos
    int ex = sx + m0, ey = sy + n0; // end pos
    ofs << "Start Position: (" << sx << ", " << sy << ")\n";
    ofs << "End Position: (" << ex << ", " << ey << ")\n";
    ofs << "Block Matrix Elements:\n";
    ofs << M << "\n";
    ofs.close();
}

// save the result
void mpi_output(string out_file, int m, int n, Mat_Demo<double> &M){
    std::ofstream ofs(out_file);
    if(!ofs.is_open()){
        std::cerr << "Failed to open file \'" << out_file << "\' \n";
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    ofs << "nrows: " << m << "\n";
    ofs << "ncols: " << n << "\n";
    ofs << M << "\n";
    ofs.close();
}