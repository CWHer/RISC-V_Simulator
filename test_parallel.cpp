#include"instruction.hpp"
#include"execute.hpp"
#include"register.hpp"
#include"memory.hpp"
#include"RISC-V.h"
#include<iostream>
Register reg;
Instruction opt;
Memory mem;
int main()
{
    int cnt=0;
    // freopen("out","w",stdout);
    mem.init_read();
    // freopen("in","r",stdin);
    opt.fetch(&mem,&reg);
    while (!opt.isEnd())
    {
        opt.decode();
        // cnt++;
        // std::cout<<++cnt<<' '<<str[opt.gettype()]<<std::endl;
        // std::cout<<reg.output()<<std::endl;
        // if (cnt==31855)
        // {
            // puts("1");
        // }
        // if (opt.empty()) break;
        Execute exe(&opt,&reg,&mem);
        exe.run();
        exe.memory_access();
        exe.write_back();
        opt.fetch(&mem,&reg);
    }
    std::cout<<cnt<<std::endl;
    std::cout<<reg.output()<<std::endl;
    return 0;
}