#include"RISC-V.hpp"
#include"memory.hpp"
#include<iostream>
Memory mem;
RISC_V unit(&mem);
int main()
{
    unit.run();
    std::cout<<unit.exetimes()<<std::endl;
    std::cout<<unit.output()<<std::endl;
}