#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include <ctime>
#include <cstdlib>
#include <sstream>

#include "input.h"
#include "mat_demo.h"
#include "mpi.h"

using std::string;

string to_lower(const string &src){
    string res = "";
    for(char c : src)
        res += tolower(c);
    return res;
}

Input::Input(){}
Input::~Input(){}

string Input::getarg(const string &key){
    if(!args.count(key)){
        std::cerr << "missing \'" << key << "\'\n";
        #ifdef __MPI
            MPI_Abort(MPI_COMM_WORLD, 1);
        #else
            abort();
        #endif
    }
    return args[key];
}

void Input::init(const string &fname){
    std::ifstream ifs(fname, std::ios::in); // Create ifs object
    if(!ifs.is_open()){
        std::cerr << "Failed to open file \'" << fname << "\'\n";
        #ifdef __MPI
            MPI_Abort(MPI_COMM_WORLD, 1);
        #else
            abort();
        #endif
    }

    for(string line = "";std::getline(ifs, line);){
        int ptr = line.find('#');    // find possible '#'
        if(ptr != string::npos) line = line.substr(0, ptr); // truncate
        
        std::istringstream ss(line);    // using sstream for param parsing
        string argname, argval;
        ss >> argname >> argval;
        args[argname] = argval;
    }
}

Mat_Demo<double> Input::load_matrix(const string &fname){
    std::ifstream ifs(fname, std::ios::in); // Create ifs object
    if(!ifs.is_open()){
        std::cerr << "Failed to open file \'" << fname << "\'\n";
        #ifdef __MPI
            MPI_Abort(MPI_COMM_WORLD, 1);
        #else
            abort();
        #endif
    }

    Mat_Demo<double> ret(0, 0);

    std::map<string, string> attr;  // attributes of matrix

    for(string line = "";std::getline(ifs, line);){
        int ptr = line.find('#');    // find possible '#'
        if(ptr != string::npos) line = line.substr(0, ptr); // truncate
        
        std::istringstream ss(line);    // using sstream for param parsing
        string argname, argval;
        ss >> argname;
        if(argname != "value:"){
            ss >> argval;
            attr[argname] = argval;
        }else{
            auto getattr = [&](const string &name){
                if(!attr.count(name)){
                    std::cerr << fname << ": missing \'" << name << "\'\n";
                    #ifdef __MPI
                        MPI_Abort(MPI_COMM_WORLD, 1);
                    #else
                        abort();
                    #endif
                }
                return attr[name];
            };  // get value with check
            
            // Reading matrix elements
            // We assume that attributes is all loaded previously
            int nr = std::stoi(getattr("nrows:"));
            int nc = std::stoi(getattr("ncols:"));
            string type = getattr("type:");
            auto othch = [](char c){
                return !isdigit(c) \
                    && c != '.' \
                    && c != '-' \
                    && c != '(' && c != ')';
            }; // Other char to be ignored
            ret.resize(nr, nc);
            for(int i = 0;i < nr;i++) for(int j = 0;j < nc;j++){
                for(int c = ifs.peek(); !ifs.eof() && othch(c); c = ifs.peek())
                        ifs.get();
                    // ignore some chars
                ifs >> ret(i, j);
            }
        }
    }
    return ret;
}

string Input::operator [] (const string &key){
    return getarg(key);
}