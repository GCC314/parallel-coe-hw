#include <iostream>
#include <fstream>
#include <map>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <complex>
#include <cassert>

#include <cblas.h>
#include <lapacke.h>
#include "mytimer.h"
#include "mat_demo.h"

Mat_Demo::Mat_Demo(): nrows(0), ncols(0){}

Mat_Demo::Mat_Demo(const int &_nrows, const int &_ncols){
    nrows = _nrows;
    ncols = _ncols;
    if(nrows == 0 || ncols == 0) return;
    d = (double*) malloc(sizeof(double) * nrows * ncols);
    memset(d, 0, nrows * ncols * sizeof(double));
}

int Mat_Demo::nr() const {return nrows;}
int Mat_Demo::nc() const {return ncols;}
void Mat_Demo::resize(int _nr, int _nc){
    // Discard all the previous data
    if(d != nullptr) free(d);
    nrows = _nr;ncols = _nc;
    if(nrows == 0 || ncols == 0) return;
    d = (double*) malloc(sizeof(double) * nrows * ncols);
}

Mat_Demo::~Mat_Demo(){
    if(d != nullptr) free(d);
}

void Mat_Demo::operator *= (const double &s){
    for(int i = 0;i < nrows;i++){
        for(int j = 0;j < ncols;j++){
            d[i * ncols + j] *= s;
        }
    }
}

void Mat_Demo::operator = (const Mat_Demo &ano){
    if(nrows != ano.nr() || ncols != ano.nc()){
        free(d);
        nrows = ano.nr();
        ncols = ano.nc();
        d = (double*) malloc(sizeof(double) * nrows * ncols);
    }
    memcpy(d, ano.d, sizeof(double) * nrows * ncols);
}

Mat_Demo operator + (const Mat_Demo &A, const Mat_Demo &B){
    if(A.nr() != B.nr() || A.nc() != B.nc()){
        throw "Matrix size inconsistency";
    }
    int nrows = A.nr(), ncols = A.nc();
    Mat_Demo res(nrows, ncols);
    for(int i = 0, siz = nrows * ncols;i < siz;i++)
        res.d[i] = A.d[i] + B.d[i];
    return res;
}

Mat_Demo operator - (const Mat_Demo &A, const Mat_Demo &B){
    if(A.nr() != B.nr() || A.nc() != B.nc()){
        throw "Matrix size inconsistency";
    }
    int nrows = A.nr(), ncols = A.nc();
    Mat_Demo res(nrows, ncols);
    for(int i = 0, siz = nrows * ncols;i < siz;i++)
        res.d[i] = A.d[i] + B.d[i];
    return res;
}

Mat_Demo operator * (const Mat_Demo &A, const Mat_Demo &B){
    mytimer::tick("Mat_Demo", "operator *");

    if(A.nc() != B.nr()){
        throw "Matrix size inconsistency";
    }
    int M = A.nr(), K = A.nc(), N = B.nc();
    Mat_Demo res(M, N);
    for(int i = 0;i < M;i++){
        for(int j = 0;j < N;j++){
            for(int k = 0;k < K;k++){
                res(i, j) += A.get(i, k) * B.get(k, j);
            }
        }
    }

    mytimer::tick("Mat_Demo", "operator *");
    return res;
}

bool operator == (const Mat_Demo &A, const Mat_Demo &B){
    if(A.nc() != B.nc() || A.nr() != B.nr())
        return false;
    int M = A.nr(), N = A.nc();
    for(int i = 0;i < M;i++) for(int j = 0;j < N;j++){
        if(A.get(i, j) != B.get(i, j)) return false;
    }
    return true;
}

bool operator != (const Mat_Demo &A, const Mat_Demo &B){
    return !(A == B);
}

double& Mat_Demo::operator() (const int &i, const int &j){
    assert(nrows != 0 && ncols != 0);
    return d[i * ncols + j];
}

double Mat_Demo::get(const int &i, const int &j) const {
    assert(nrows != 0 && ncols != 0);
    return d[i * ncols + j];
}

// Dump the matrix to the screen
std::ostream& operator << (std::ostream& os, const Mat_Demo &mat){
    os << "nrows = " << mat.nrows << ", ";
    os << "ncols = " << mat.ncols << std::endl;
    os << std::setprecision(8);
    for(int i = 0;i < mat.nrows;i++){
        for(int j = 0;j < mat.ncols;j++){
            os << mat.d[i * mat.ncols + j] << "\t\n"[j == mat.ncols - 1];
        }
    }
    return os;
}

Mat_Demo blas_matmul(const Mat_Demo &A, const Mat_Demo &B){
    mytimer::tick("Mat_Demo", "blas_matmul");

    if(A.nc() != B.nr()){
        throw "Matrix size inconsistency";
    }
    int M = A.nr(), K = A.nc(), N = B.nc();
    Mat_Demo res(M, N);
    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        M, N, K,
        1.0, A.d, K, B.d, N, 0.0, res.d, N);

    mytimer::tick("Mat_Demo", "blas_matmul");
    return res;
}

bool Mat_Demo::isRSM(){
    if(nrows != ncols) return false;
    if(nrows == 0) return true;     // Odd but correct
    for(int i = 0;i < nrows;i++){
        for(int j = 0;j < i;j++){
            if((*this)(i, j) != (*this)(j, i))
                return false;
        }
    }
    return true;
}

bool Mat_Demo::lapack_diago(bool isprint){
    mytimer::tick("Mat_Demo", "lapack_diago");

    assert(nrows == ncols && nrows > 0);
    int N = nrows;
    double w[N];
    int res = LAPACKE_dsyev(LAPACK_ROW_MAJOR, 'V', 'U',
        N, d, N, w);

    mytimer::tick("Mat_Demo", "lapack_diago");

    if(res){    // Diagonization failed
        if(isprint)
            std::cout << "Failed to diagonize the matrix\n";
        return false;
    }
    
    // Diagonization succeeded
    if(!isprint) return true;
    std::cout << "Eigenvalues:\n";
    for(int i = 0;i < N;i++) std::cout << w[i] << " \n"[i == N - 1];
    std::cout << "Eigenvectors:\n";
    for(int i = 0;i < N;i++) for(int j = 0;j < N;j++){
        std::cout << std::setw(6) << (*this)(i, j) << " \n"[j == N - 1];
    }
    return true;
}