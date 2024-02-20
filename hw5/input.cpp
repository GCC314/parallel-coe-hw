#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include <cstdlib>
#include <ctime>

#include "input.h"

std::string to_lower(const std::string &src){
    std::string res = "";
    for(char c : src){
        res += tolower(c);
    }
    return res;
}

Input::Input(){}
Input::~Input(){}

std::string Input::getval(std::string key){
    if(!args.count(key)){
        std::cerr << "No required argument \'" << key << "\'\n";
        abort();
    }
    return args[key];
}

void Input::init(const std::string &fname, std::ofstream &ofs){
    std::ifstream ifs(fname, std::ios::in); // Create ifs object
    if(!ifs.is_open()){
        std::cerr << "Failed to open \'" << fname << "\'\n";
        abort();
    }

    // Reading from the ifstream line by line
    for(std::string line = ""; std::getline(ifs, line); ){
        // Parsing arguments
        line += " ";                // make parsing happy
        std::string cur = "";       // the current word
        std::string varname = "";   // temporarily store varname
        int argcnt = 0;             // count of args
        for(char c : line){
            if(!isspace(c)){        // normal char
                cur += c;
            }else{                  // delim
                argcnt += 1;
                if(argcnt == 1){
                    varname = to_lower(cur);
                    cur = "";
                }else{
                    args[varname] = to_lower(cur);
                    break;          // not necessary to read forward
                }
            }
        }
        if(argcnt < 2){
            std::cerr << "Invalid line: " << line << std::endl;
        }
    }
    
    // Choosing requisite args
    cal = getval("calculation");
    mat_typ = getval("matrix_type");
    mat_prt = std::stoi(getval("matrix_print"));
    nr = std::stoi(getval("nrows"));
    nc = std::stoi(getval("ncols"));

    // Set ofstream
    if(mat_prt){
        // get timestamp as log filename
        char namebuf[50];
        time_t rawtime = time(NULL);
        strftime(namebuf, 50, "%Y-%m-%d-%H-%M-%S", localtime(&rawtime));
        std::string outfname = "log-" + std::string(namebuf) + ".log";
        ofs.open(outfname, std::ios::out);
        if(!ofs.is_open()){
            std::cerr << "Failed to open \'" << outfname << "\'\n";
            abort();
        }
    }
}

std::ostream& operator << (std::ostream& os, const Input& inp){
    os << "Input::calc = " << inp.cal << std::endl;
    os << "Input::mat_typ = " << inp.mat_typ << std::endl;
    os << "Input::nr = " << inp.nr << std::endl;
    os << "Input::nc = " << inp.nc << std::endl;
    os << "Input::mat_prt = " << inp.mat_prt << std::endl;
    return os;
}