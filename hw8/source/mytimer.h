#ifndef __MYTIMER_H__
#define __MYTIMER_H__

#include <map>
#include <ctime>
#include <utility>
#include <iostream>
#include <cstring>

typedef std::pair<std::string, std::string> func_t;
typedef unsigned long long u64;

struct fun_status{
    double prev_ts;
    bool running;
    double tsum;
    u64 cnt;
    fun_status(double p = 0, bool r = false, double t = 0, u64 c = 0): \
        prev_ts(p), running(r), tsum(t), cnt(c) {}
};

class mytimer{
    public:
        static void print();
        static void tick(const std::string &cname, const std::string &fname);
        static void init();
        static void end();
    private:
        static std::map<func_t, fun_status> status;
        static double start_timestamp;
        static double end_timestamp;
};

#endif