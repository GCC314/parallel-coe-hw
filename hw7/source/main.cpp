#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <cassert>

#include <cblas.h>
#include <lapacke.h>

#include "mat_demo.h"
#include "input.h"
#include "mytimer.h"

using std::string;

int main(int argc, char **argv){
    if(argc != 2){
        std::cerr << "Usage: " << argv[0] << " [FILE]" << std::endl;
        return 1;
    }
    Input inp;
    inp.init(string(argv[1]));

    assert(inp["matrix_type"] == "file"); // just a check
    
    string cal = inp["calculation"];
    Mat_Demo mat1, mat2;
    if(cal == "matmul"){
        // matmul task, 2 mats
        mat1 = inp.load_matrix(inp["matrix_1"]);
        mat2 = inp.load_matrix(inp["matrix_2"]);
    }else if(cal == "rsmdiago"){
        // diago task, 1 mat
        mat1 = inp.load_matrix(inp["matrix_1"]);
    }else{
        std::cerr << "Calculation \'" << cal << "\' not supported\n";
        abort();
    }

    mytimer::init();
    // Below are the computation tasks

    if(cal == "matmul"){
        // matmul
        Mat_Demo ret_my = mat1 * mat2;
        Mat_Demo ret_blas = blas_matmul(mat1, mat2);
        if(ret_my != ret_blas){
            std::cout << "Matmul results are inconsistent!" << std::endl;
        }else{
            std::cout << "Matmul results are consistent." << std::endl;
        }
        if(inp["result_print"] == "1"){
            std::cout << "ret_my:" << std::endl;
            std::cout << ret_my << std::endl;
            std::cout << "ret_blas:" << std::endl;
            std::cout << ret_blas << std::endl;
        }
    }else{
        int isprint = (inp["result_print"] == "1");
        if(!mat1.isRSM()){
            std::cout << "matrix is not a RSM" << std::endl;
        }else{
            mat1.lapack_diago(isprint);
        }
    }

    // Above are the computation tasks
    mytimer::end();
    // print timer infos
    if(inp["timer_print"] != "1") return 0;
    mytimer::print();
    return 0;
}