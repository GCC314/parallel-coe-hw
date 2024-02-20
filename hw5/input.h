#ifndef __INPUT_H__
#define __INPUT_H__

#include <cstring>
#include <iostream>
#include <fstream>
#include <map>

class Input{
    public:
        Input();
        ~Input();
        
        void init(
            const std::string &fname,
            std::ofstream &ofs
        );

        std::string cal;        // type of calc
        std::string mat_typ;    // type of matrix
        int nr;                 // number of rows
        int nc;                 // number of columns
        int mat_prt;            // the way to print matrix
                                // assume output to std::cout when == 0
                                // output to specified ofs when != 0

        friend std::ostream& operator << (std::ostream& os,const Input &inp);
    
    private:
        std::map<std::string, std::string> args;
        std::string getval(std::string key);
};

#endif