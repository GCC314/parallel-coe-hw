#ifndef __MAT_DEMO_H__
#define __MAT_DEMO_H__

#include <iostream>
#include <fstream>
#include <map>
#include <complex>

class Mat_Demo{
    public:

        int nr(void) const;
        int nc(void) const;
        void resize(int _nr, int _nc);

        double *d = nullptr;    // data

        Mat_Demo();
        Mat_Demo(const int &_nrows, const int &_ncols);

        ~Mat_Demo();

        void operator *= (const double &s);
        void operator = (const Mat_Demo &ano);
        friend Mat_Demo operator + (const Mat_Demo &A, const Mat_Demo &B);
        friend Mat_Demo operator - (const Mat_Demo &A, const Mat_Demo &B);
        friend Mat_Demo operator * (const Mat_Demo &A, const Mat_Demo &B);
        friend bool operator == (const Mat_Demo &A, const Mat_Demo &B);
        friend bool operator != (const Mat_Demo &A, const Mat_Demo &B);
        double& operator () (const int &i, const int &j);
        double get(const int &i, const int &j) const ;

        friend std::ostream& operator << (std::ostream& os,const Mat_Demo &mat);


    private:
        int nrows;              // number of rows
        int ncols;              // number of columns
};

#endif