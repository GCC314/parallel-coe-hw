#ifndef __MAT_DEMO_H__
#define __MAT_DEMO_H__

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
#include "mpi.h"

template <typename T> class Mat_Demo;
template <typename T>
Mat_Demo<T> operator + (const Mat_Demo<T> &A, const Mat_Demo<T> &B);
template <typename T>
Mat_Demo<T> operator - (const Mat_Demo<T> &A, const Mat_Demo<T> &B);
template <typename T>
Mat_Demo<T> operator * (const Mat_Demo<T> &A, const Mat_Demo<T> &B);
template <typename T>
bool operator == (const Mat_Demo<T> &A, const Mat_Demo<T> &B);
template <typename T>
bool operator != (const Mat_Demo<T> &A, const Mat_Demo<T> &B);
template <typename T>
std::ostream& operator << (std::ostream& os, const Mat_Demo<T> &mat);

template<class T>
class Mat_Demo{
    public:

        Mat_Demo();
        Mat_Demo(const int &_nrows, const int &_ncols);
        Mat_Demo(const Mat_Demo &ano);
        ~Mat_Demo();

        int nr(void) const;
        int nc(void) const;
        void resize(int _nr, int _nc);

        T *d = nullptr;    // data


        void operator *= (const T &s);
        void operator = (const Mat_Demo<T> &ano);
        friend Mat_Demo operator + <> (const Mat_Demo &A, const Mat_Demo &B);
        friend Mat_Demo operator - <> (const Mat_Demo &A, const Mat_Demo &B);
        friend Mat_Demo operator * <> (const Mat_Demo &A, const Mat_Demo &B);
        friend bool operator == <> (const Mat_Demo &A, const Mat_Demo &B);
        friend bool operator != <> (const Mat_Demo &A, const Mat_Demo &B);
        T& operator () (const int &i, const int &j);
        T get(const int &i, const int &j) const ;

        friend std::ostream& operator << <> (std::ostream& os, const Mat_Demo &mat);

        void copy(const Mat_Demo &ano, int ox = 0, int oy = 0);
        void copyfrom(const Mat_Demo &ano, int ox = 0, int oy = 0);
    private:
        int nrows;              // number of rows
        int ncols;              // number of columns
};

template<class T>
Mat_Demo<T>::Mat_Demo(): nrows(0), ncols(0), d(nullptr){}

template<class T>
Mat_Demo<T>::Mat_Demo(const int &_nrows, const int &_ncols){
    nrows = _nrows;
    ncols = _ncols;
    d = nullptr;
    if(nrows == 0 || ncols == 0) return;
    d = (T*) malloc(sizeof(T) * nrows * ncols);
    memset(d, 0, nrows * ncols * sizeof(T));
}

template<class T>
Mat_Demo<T>::Mat_Demo(const Mat_Demo &ano){
    d = nullptr;
    nrows = ano.nr();
    ncols = ano.nc();
    if(!(nrows * ncols)) return;
    d = (T*) malloc(sizeof(T) * nrows * ncols);
    memcpy(d, ano.d, sizeof(T) * nrows * ncols);
}

template<class T>
int Mat_Demo<T>::nr() const {return nrows;}

template<class T>
int Mat_Demo<T>::nc() const {return ncols;}

template<class T>
void Mat_Demo<T>::resize(int _nr, int _nc){
    // Discard all the previous data
    if(d != nullptr){
        free(d);
        d = nullptr;
    }
    nrows = _nr;ncols = _nc;
    if(nrows == 0 || ncols == 0) return;
    d = (T*) malloc(sizeof(T) * nrows * ncols);
}

template<class T>
Mat_Demo<T>::~Mat_Demo(){
    if(d != nullptr) free(d);
}

template<class T>
void Mat_Demo<T>::operator *= (const T &s){
    for(int i = 0;i < nrows;i++){
        for(int j = 0;j < ncols;j++){
            d[i * ncols + j] *= s;
        }
    }
}

template<class T>
void Mat_Demo<T>::operator = (const Mat_Demo<T> &ano){
    if(nrows != ano.nr() || ncols != ano.nc()){
        if(nrows * ncols){
            free(d);
            d = nullptr;
        }
        nrows = ano.nr();
        ncols = ano.nc();
        if(!(nrows * ncols)) return;
        d = (T*) malloc(sizeof(T) * nrows * ncols);
    }
    memcpy(d, ano.d, sizeof(T) * nrows * ncols);
}

template<typename T>
Mat_Demo<T> operator + (const Mat_Demo<T> &A, const Mat_Demo<T> &B){
    if(A.nr() != B.nr() || A.nc() != B.nc()){
        throw "Matrix size inconsistency";
    }
    int nrows = A.nr(), ncols = A.nc();
    Mat_Demo<T> res(nrows, ncols);
    for(int i = 0, siz = nrows * ncols;i < siz;i++)
        res.d[i] = A.d[i] + B.d[i];
    return res;
}

template<typename T>
Mat_Demo<T> operator - (const Mat_Demo<T> &A, const Mat_Demo<T> &B){
    if(A.nr() != B.nr() || A.nc() != B.nc()){
        throw "Matrix size inconsistency";
    }
    int nrows = A.nr(), ncols = A.nc();
    Mat_Demo<T> res(nrows, ncols);
    for(int i = 0, siz = nrows * ncols;i < siz;i++)
        res.d[i] = A.d[i] + B.d[i];
    return res;
}

template<typename T>
Mat_Demo<T> operator * (const Mat_Demo<T> &A, const Mat_Demo<T> &B){
    // mytimer::tick("Mat_Demo", "operator *");

    if(A.nc() != B.nr()){
        throw "Matrix size inconsistency";
    }
    int M = A.nr(), K = A.nc(), N = B.nc();
    Mat_Demo<T> res(M, N);
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

template<typename T>
bool operator == (const Mat_Demo<T> &A, const Mat_Demo<T> &B){
    if(A.nc() != B.nc() || A.nr() != B.nr())
        return false;
    int M = A.nr(), N = A.nc();
    for(int i = 0;i < M;i++) for(int j = 0;j < N;j++){
        if(A.get(i, j) != B.get(i, j)) return false;
    }
    return true;
}

template<typename T>
bool operator != (const Mat_Demo<T> &A, const Mat_Demo<T> &B){
    return !(A == B);
}

template<typename T>
T& Mat_Demo<T>::operator() (const int &i, const int &j){
    if(nrows == 0 || ncols == 0){
        std::cerr << "get() from empty matrix\n";
        #ifdef __MPI
            MPI_Abort(MPI_COMM_WORLD, 1);
        #else
            abort();
        #endif
    }
    return d[i * ncols + j];
}

template<typename T>
T Mat_Demo<T>::get(const int &i, const int &j) const {
    if(nrows == 0 || ncols == 0){
        std::cerr << "get() from empty matrix\n";
        #ifdef __MPI
            MPI_Abort(MPI_COMM_WORLD, 1);
        #else
            abort();
        #endif
    }
    return d[i * ncols + j];
}

// Dump the matrix to the screen
template<typename T>
std::ostream& operator << (std::ostream& os, const Mat_Demo<T> &mat){
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


// chop a same-size area in ano, with offset = (ox, oy)
// to copy into *this
// (*this not bigger than ano)
template <typename T>
void Mat_Demo<T>::copy(const Mat_Demo<T> &ano, int ox, int oy){
    for(int x = 0;x < nrows;x++) for(int y = 0;y < ncols;y++)
        (*this)(x, y) = ano.get(x + ox, y + oy);
}


// paste ano to *this, with offset = (ox, oy)
// (*this not smaller than ano)
template <typename T>
void Mat_Demo<T>::copyfrom(const Mat_Demo<T> &ano, int ox, int oy){
    for(int x = 0;x < ano.nrows;x++) for(int y = 0;y < ano.ncols;y++)
        (*this)(x + ox, y + oy) = ano.get(x, y);
}

#endif