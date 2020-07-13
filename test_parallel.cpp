#include"register.hpp"
#include"memory.hpp"
#include"predictor.hpp"
#include"RISC-V.h"
#include"instructionfetch.hpp"
#include"instructiondecode.hpp"
#include"execute.hpp"
#include"memoryaccess.hpp"
#include"writeback.hpp"
#include<iostream>
#include<iomanip>
#include<cstdio>
Register reg;
Memory mem;
Predictor prd;
InstructionFetch IF;
InstructionDecode ID(&prd);
Execute EXE;
MemoryAccess MEM;
WriteBack WB(1);
int main()
{
    // std::ios::sync_with_stdio(0);
    // freopen("ans","w",stdout);
    // freopen("out","w",stdout);
    bool MEM2WB,isPB,isStall;     //whether MEM->WB 
    mem.init_read();
    IF.init(&mem,&reg);
    int cnt=0,wcnt=0;
    do {
        // std::cout<<++cnt<<' '<<str[ID.gettype()]<<std::endl;
        // std::cout<<reg.output()<<std::endl;
        // if (cnt==613557)
        // {
        //     puts("1");
        // }
        // if (opt.empty()) break;
        ++cnt,isPB=isStall=0;
        WB.run();
        MEM.run();
        MEM2WB=MEM.gettype()!=EMPTY;
        WB.init(MEM);
        if (!MEM.isLock()&&MEM.gettype()!=EMPTY) MEM.forwarding(EXE);
        EXE.run();
        if (!EXE.check(wcnt))    //put back pipeline to last clk
        {                        //jump&incorrect pred
            reg.prevpc();
            EXE.putback(ID);
            EXE.reset();
            ID.setJump();
            isPB=1;
        }

        if (EXE.willJump()&&!isPB) EXE.forwarding(IF);

        EXE.update(&prd);
        MEM.init(EXE);
        if (EXE.gettype()!=EMPTY||EXE.isEnd())  
        {
            if (!isSL(EXE.gettype()))   //skip MEM if without SL
            {
                if (MEM2WB)     //WAW
                    isStall=1;  
                else 
                    WB.init(MEM);  
                MEM.reset();    
            }
            else MEM.putwclk(3);
        }
        if (!isPB&&!isStall) ID.run();

        // if (ID.gettype()!=EMPTY)
        // {
        //     std::cout<<str[ID.gettype()]<<'\n';
        //     reg.printdata();
        // }
        // std::cout<<str[ID.gettype()]<<std::endl;
        // reg.printdata();

        // if (isJump(ID.gettype()))
        if (ID.willJump())
        {
            IF.reset();
            IF.putwclk(2);  //1+1+1 without SL
            // IF.putwclk(6);
        }
        if (isSL(ID.gettype()))
        {
            IF.reset();
            IF.putwclk(3);  //3+1
        }
        if (!isStall) EXE.init(ID);
        if (!isStall) IF.run();
        // std::cout<<reg.getpc()<<std::endl;
        if (!isStall) ID.init(IF);
    } while (!WB.isEnd());
    std::cout<<prd.tot-wcnt<<'/'<<prd.tot<<' ';
    std::cout<<std::setprecision(2)<<(double)(prd.tot-wcnt)/prd.tot<<std::endl;
    std::cout<<cnt<<std::endl;
    std::cout<<reg.output()<<std::endl;
    return 0;
}