#ifndef __MYTIMER_H__
#define __MYTIMER_H__

#include <map>
#include <ctime>
#include <utility>
#include <iostream>
#include <cstring>

typedef std::pair<std::string, std::string> func_t;
typedef unsigned long long u64;         // represent time length

struct fun_status{
    clock_t prev_ts;
    bool running;
    u64 tsum;
    u64 cnt;
    fun_status(clock_t p = 0, bool r = false, u64 t = 0, u64 c = 0): \
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
        static clock_t start_timestamp;
        static clock_t end_timestamp;
};

#endif