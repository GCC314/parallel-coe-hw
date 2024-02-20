// requires -std=c++11

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <map>
#include <vector>
#include <cctype>

std::string to_lower(const std::string &src){
    std::string res = "";
    for(char c : src){
        res += tolower(c);
    }
    return res;
}

int main(int argc, char **argv){
    
    // Get the file name from args
    if(argc != 2){
        std::cerr << "Usage: " << argv[0] << " [FILE]" << std::endl;
        return 1;
    }

    std::ifstream ifs(argv[1], std::ios::in); // Create ifs object
    if(!ifs.is_open()){
        std::cerr << "Failed to open \'" << argv[1] << "\'" << std::endl;
        return 1;
    }
    
    std::map<std::string, std::string> argdict;

    // Reading from the fstream line by line
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
                }else{              // argcnt == 2
                    argdict[varname] = to_lower(cur);
                    break;          // not necessary to read forward
                                    // we just require the first two words
                                    // because of the grammar
                }
            }
        }
        if(argcnt < 2){
            std::cerr << "Invalid line: " << line << std::endl;
        }
    }

    // Print required variables
    std::vector<std::string> required_vars({
        "calculations", 
        "matrix_type",
        "matrix_print",
        "nrows",
        "ncols"
    });
    for(std::string var : required_vars){
        std::cout << var << ": " << argdict[var] << std::endl;
    }

    return 0;
}
