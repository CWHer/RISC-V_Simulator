#include"RISC-V.hpp"
#include"memory.hpp"
#include<iostream>
Memory mem;
RISC_V unit0(&mem,0);    //mode  0(default):serial   1:parallel
RISC_V unit1(&mem,1);
int main()
{
    unit0.run();
    std::cout<<unit0.clktimes()<<std::endl;
    std::cout<<unit0.output()<<std::endl;
    // unit1.run();
    // std::cout<<unit1.clktimes()<<std::endl;
    // std::cout<<unit1.output()<<std::endl;
    return 0;
}