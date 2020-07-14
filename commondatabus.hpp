#ifndef __COMMONDATABUS__
#define __COMMONDATABUS__

#include"RISC-V.h"
#include"executor.hpp"
#include"register.hpp"

class CommonDataBus
{
    private:
        struct cmp
        {
            bool operator() (const Executor &a,const Executor &b)
            {
                return a.opt.num<b.opt.num;     //I'm not sure 
            } 
        };
        std::priority_queue<Executor,std::vector<Executor>,cmp> Q;
    public:
        void push(Executor t)
        {
            Q.push(t);
        }
        void run(Register *reg)
        {
            Q.top().write_back(reg);
            Q.pop();
        }
        bool empty()
        {
            return Q.empty();
        }
};

#endif