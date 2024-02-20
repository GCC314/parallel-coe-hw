#ifndef __MAT_DEMO_H__
#define __MAT_DEMO_H__

#include <iostream>
#include <fstream>
#include <map>

class Mat_Demo{
    public:
        enum mtype{
            MAT_ZERO,
            MAT_RANDOM,
            MAT_ONE
        };

        int nr(void) const;
        int nc(void) const;

        double *d = nullptr;    // data

        Mat_Demo();
        Mat_Demo(
            const int &_nrows,
            const int &_ncols,
            const enum mtype &type = MAT_ZERO
        );

        ~Mat_Demo();

        void zero();
        void zero(
            const int &_nrows,
            const int &_ncols
        );

        void operator *= (const double &s);
        void operator = (const Mat_Demo &ano);
        friend Mat_Demo operator + (const Mat_Demo &A, const Mat_Demo &B);
        friend Mat_Demo operator - (const Mat_Demo &A, const Mat_Demo &B);

        double Max();
        double Min();

        friend std::ostream& operator << (std::ostream& os,const Mat_Demo &mat);

    private:
        int nrows;              // number of rows
        int ncols;              // number of columns
};

#endif