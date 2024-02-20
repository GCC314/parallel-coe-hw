#ifndef __INPUT_H__
#define __INPUT_H__

#include <cstring>
#include <iostream>
#include <fstream>
#include <map>

#include "mat_demo.h"

using std::string;

class Input{
    public:
        Input();
        ~Input();
        
        void init(const string &fname);
        Mat_Demo load_matrix(const string &fname);
        string getarg(const string &key);
        // Syntactic sugar for getarg
        string operator [] (const string &key);
    
    private:
        std::map<string, string> args;
};

#endif