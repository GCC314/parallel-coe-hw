#include "mytimer.h"

#include <map>
#include <ctime>
#include <utility>
#include <iostream>
#include <cstring>
#include <iomanip>
#include "mpi.h"

#ifdef DEBUG
#include <cassert>
#endif

std::map<func_t, fun_status> mytimer::status;
double mytimer::start_timestamp;
double mytimer::end_timestamp;

/*

TABLE FORMAT:

name            width
CLASS_NAME      20
NAME            20
TIME(Sec)       15
CALLS           10
AVG             15
PERF%           12

total           87
*/

void mytimer::print(){
    using std::cout;
    using std::setw;
    using std::setprecision;

    // We only output elapsed time in this hw
    cout << "Time elapsed " << setprecision(5);
    cout << (end_timestamp - start_timestamp);
    cout << " s\n";
    return;

    // Header
    cout << "\n";
    cout << setw(20) << "CLASS_NAME|";
    cout << setw(20) << "NAME|";
    cout << setw(15) << "TIME(Sec)|";
    cout << setw(10) << "CALLS|";
    cout << setw(15) << "AVG|";
    cout << setw(12) << "PERF%";
    cout << "\n"; 

    // Total
    double tottime = (end_timestamp - start_timestamp);

    cout << setw(20) << "";
    cout << setw(20) << "total";
    cout << setw(15) << setprecision(6) << tottime;
    cout << setw(10) << "";
    cout << setw(15) << "";
    cout << setw(12) << "";
    cout << "\n";

    // Each func
    for(auto node : status){
        auto &fun = node.first;
        auto &sta = node.second;

        cout << setw(20) << fun.first;
        cout << setw(20) << fun.second;
        cout << std::resetiosflags(std::ios::fixed);
        cout << setw(15) << setprecision(6) << sta.tsum;
        cout << setw(10) << sta.cnt;
        cout << setw(15) << setprecision(6) << sta.tsum / sta.cnt;
        cout << setw(12) << std::fixed << setprecision(1) \
            << 100.0 * sta.tsum / tottime << "%";
        cout << "\n";
    }

    // Footer
    cout << std::string(92, '-');
}

void mytimer::tick(const std::string &cname, const std::string &fname){
    func_t fun = make_pair(cname, fname);
    double now = 0;
    #ifdef __MPI
        now = MPI_Wtime();
    #else
        now = clock() / CLOCKS_PER_SEC;
    #endif
    if(status.find(fun) == status.end()){
        // Not even visited before
        status[fun] = fun_status(now, true);
    }else if(!status[fun].running){
        // Not running currently
        fun_status &sta = status[fun];
        sta.prev_ts = now;
        sta.running = true;
    }else{
        // Running currently
        fun_status &sta = status[fun];
        sta.tsum += now - sta.prev_ts;
        sta.prev_ts = 0;
        ++sta.cnt;
        sta.running = false;
    }
}

void mytimer::init(){
    #ifdef __MPI
        mytimer::start_timestamp = MPI_Wtime();
    #else
        mytimer::start_timestamp = ((double)clock()) / CLOCKS_PER_SEC;
    #endif
    status.clear();
}

void mytimer::end(){

    #ifdef DEBUG
        for(auto node : status){
            if(node.second.running){
                std::cerr << node.first.first << "." << node.first.second \
                    << "still running!" << std::endl;
                assert(false);
            }
        }
        std::cerr << "Tick parity check ok!" << std::endl;
    #endif

    #ifdef __MPI
        mytimer::end_timestamp = MPI_Wtime();
    #else
        mytimer::end_timestamp = ((double)clock()) / CLOCKS_PER_SEC;
    #endif
}