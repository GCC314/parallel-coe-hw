#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <cstdlib>

#include "mat_demo.h"
#include "mytimer.h"

int main(int argc, char **argv){

    mytimer::init();

    // Just doing benchmark
    const int msize = 250;
    const int loop_time = 1000;
    Mat_Demo mat0(msize, msize);
    double something = 1e10;
    double another = -1e10;
    for(int c = 0;c < loop_time;c++){
        Mat_Demo matm(msize, msize, Mat_Demo::MAT_RANDOM);
        double coef = 1.0 * rand() / RAND_MAX;
        matm *= coef;
        mat0 = mat0 + matm;
        something = std::min(something, mat0.Max());
        another = std::max(another, mat0.Min());
    }

    mytimer::end();

    mytimer::print();

    return 0;
}