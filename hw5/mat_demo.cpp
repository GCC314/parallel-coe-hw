#include <iostream>
#include <fstream>
#include <map>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <cassert>

#include "mat_demo.h"

const double rand_min = -1.0;
const double rand_max = 1.0;

inline double drand(){ // generate random in [randmin, randmax]
    return rand_min + 1.0 * rand() / RAND_MAX * (rand_max - rand_min);
}

Mat_Demo::Mat_Demo(): nrows(0), ncols(0){}

Mat_Demo::Mat_Demo(
    const int &_nrows, 
    const int &_ncols,
    const enum mtype &type
){
    nrows = _nrows;
    ncols = _ncols;
    if(nrows == 0 || ncols == 0) return;
    d = (double*) malloc(sizeof(double) * nrows * ncols);
    if(type == MAT_ZERO){
        std::fill(d, d + nrows * ncols, (double)0.0);
    }else if(type == MAT_ONE){
        std::fill(d, d + nrows * ncols, (double)1.0);
    }else{
        srand(time(NULL));
        for(int i = 0;i < nrows;i++){
            for(int j = 0;j < ncols;j++){
                d[i * ncols + j] = drand();
            }
        }
    }
}

int Mat_Demo::nr() const {return nrows;}
int Mat_Demo::nc() const {return ncols;}

Mat_Demo::~Mat_Demo(){
    if(d != nullptr) free(d);
}

void Mat_Demo::zero(){
    if(d == nullptr) return;
    std::fill(d, d + nrows * ncols, (double)0.0);
}

void Mat_Demo::zero(const int &_nrows, const int &_ncols){
    if(d != nullptr) free(d);
    nrows = _nrows;
    ncols = _ncols;
    d = (double*) malloc(sizeof(double) * nrows * ncols);
    std::fill(d, d + nrows * ncols, (double)0.0);
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
        std::cerr << "Matrix size inconsistency" << std::endl;
        abort();
    }
    int nrows = A.nr(), ncols = A.nc();
    Mat_Demo res(nrows, ncols);
    for(int i = 0, siz = nrows * ncols;i < siz;i++)
        res.d[i] = A.d[i] + B.d[i];
    return res;
}

Mat_Demo operator - (const Mat_Demo &A, const Mat_Demo &B){
    if(A.nr() != B.nr() || A.nc() != B.nc()){
        std::cerr << "Matrix size inconsistency" << std::endl;
        abort();
    }
    int nrows = A.nr(), ncols = A.nc();
    Mat_Demo res(nrows, ncols);
    for(int i = 0, siz = nrows * ncols;i < siz;i++)
        res.d[i] = A.d[i] + B.d[i];
    return res;
}

double Mat_Demo::Max(){
    assert(nrows != 0 && ncols != 0);
    double res = d[0];
    for(int i = 1, siz = nrows * ncols;i < siz;i++)
        res = std::max(res, d[i]);
    return res;
}

double Mat_Demo::Min(){
    assert(nrows != 0 && ncols != 0);
    double res = d[0];
    for(int i = 1, siz = nrows * ncols;i < siz;i++)
        res = std::min(res, d[i]);
    return res;
}

double& Mat_Demo::operator() (const int &i, const int &j){
    assert(nrows != 0 && ncols != 0);
    return d[i * ncols + j];
}

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