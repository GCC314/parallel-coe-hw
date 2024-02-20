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

// #include <cblas.h>
// #include <lapacke.h>

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
    // mytimer::tick("Mat_Demo", "operator *");

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

    // mytimer::tick("Mat_Demo", "operator *");
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
    os << std::setprecision(8);
    for(int i = 0;i < mat.nrows;i++){
        for(int j = 0;j < mat.ncols;j++){
            os << mat.d[i * mat.ncols + j];
            if(j < mat.ncols - 1) os << ",";
            os << " \n"[j == mat.ncols - 1];
        }
    }
    return os;
}