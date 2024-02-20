#include "mytimer.h"

#include <map>
#include <ctime>
#include <utility>
#include <iostream>
#include <cstring>
#include <iomanip>

#ifdef DEBUG
#include <cassert>
#endif

std::map<func_t, fun_status> mytimer::status;
clock_t mytimer::start_timestamp;
clock_t mytimer::end_timestamp;

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
    double tottime = ((double)(end_timestamp - start_timestamp)) \
        / CLOCKS_PER_SEC;

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

        double timesum = ((double)sta.tsum) / CLOCKS_PER_SEC;

        cout << setw(20) << fun.first;
        cout << setw(20) << fun.second;
        cout << std::resetiosflags(std::ios::fixed);
        cout << setw(15) << setprecision(6) << timesum;
        cout << setw(10) << sta.cnt;
        cout << setw(15) << setprecision(6) << timesum / sta.cnt;
        cout << setw(12) << std::fixed << setprecision(1) \
            << 100.0 * timesum / tottime << "%";
        cout << "\n";
    }

    // Footer
    cout << std::string(92, '-');
}

void mytimer::tick(const std::string &cname, const std::string &fname){
    func_t fun = make_pair(cname, fname);
    time_t now = clock();
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
    mytimer::start_timestamp = clock();
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

    mytimer::end_timestamp = clock();
}