#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <iomanip>
#include <cstring>

#include "input.h"
#include "mat_demo.h"

int main(int argc, char **argv){
    // Get the file name from args
    if(argc != 2){
        std::cerr << "Usage: " << argv[0] << " [FILE]" << std::endl;
        return 1;
    }
    Input inp;
    std::ofstream ofs;
    inp.init(std::string(argv[1]), ofs);

    std::ostream &os = inp.mat_prt ? ofs : std::cout;
    // Decide which stream to output to

    os << "Input :" << std::endl;
    os << inp << std::endl;
    os << std::endl;

    Mat_Demo::mtype matA_type = Mat_Demo::MAT_ZERO;
    if(inp.mat_typ == "one"){
        matA_type = Mat_Demo::MAT_ONE;
    }else if(inp.mat_typ == "random"){
        matA_type = Mat_Demo::MAT_RANDOM;
    }

    Mat_Demo matA(inp.nr, inp.nc, matA_type);
    Mat_Demo matB(inp.nr, inp.nc, Mat_Demo::MAT_ONE);
    
    matB(0, 0) = 2;     // Modify B_{00} by operator ()

    os << "Max element in matA : " << matA.Max() << std::endl;
    os << "Min element in matA : " << matA.Min() << std::endl;
    os << std::endl;

    os << "Specified calc method : " << inp.cal << std::endl;
    if(inp.cal == "max"){
        os << "result = " << matA.Max() << std::endl;
    }else{
        os << "result = " << matA.Min() << std::endl;
    }
    os << std::endl;
    
    os << "matA + matB = " << std::endl;
    os << matA + matB << std::endl;
    os << std::endl;

    if(ofs.is_open()){
        ofs.flush();
        ofs.close();
    }

    return 0;
}