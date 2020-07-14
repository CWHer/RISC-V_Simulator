#include"register.hpp"
#include"memory.hpp"
#include"predictor.hpp"

#include"issue.hpp"
#include"execute.hpp"
#include"writeback.hpp"
#include"commit.hpp"
#include<iostream>
Predictor prd;
Register reg;
Memory mem;
Execute EXE;
WriteBack WB;
int main()
{


    int cnt=0;
    // freopen("out","w",stdout);
    mem.init_read();
    IF.init(&mem,&reg);

    std::cout<<cnt<<std::endl;
    std::cout<<reg.output()<<std::endl;
    return 0;
}