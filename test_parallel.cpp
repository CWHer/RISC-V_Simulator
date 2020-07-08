#include"register.hpp"
#include"memory.hpp"
#include"RISC-V.h"
#include"instructionfetch.hpp"
#include"instructiondecode.hpp"
#include"execute.hpp"
#include"memoryaccess.hpp"
#include"writeback.hpp"
#include<iostream>
Register reg;
Memory mem;
InstructionFetch IF;
InstructionDecode ID;
Execute EXE;
MemoryAccess MEM;
WriteBack WB;

int main()
{

    mem.init_read();
    IF.init(&mem,&reg);
    int cnt=0;
    do {
        // std::cout<<++cnt<<' '<<str[ID.gettype()]<<std::endl;
        // std::cout<<reg.output()<<std::endl;
        // if (cnt==31855)
        // {
            // puts("1");
        // }
        // if (opt.empty()) break;
        ++cnt;
        WB.run();
        MEM.run();
        WB.init(MEM);
        EXE.run();
        MEM.init(EXE);
        if (EXE.gettype()!=EMPTY||EXE.isEnd())
        {
            if (!isSL(EXE.gettype()))
                WB.init(MEM),MEM.reset();
            else
                MEM.putwclk(3);
        }
        // if (!EXE.gettype()==EMPTY&&!isSL(EXE.gettype())) //when pipeline is not stalled for MEM
        //     WB.init(MEM),MEM.reset();
        // else 
        //     MEM.putwclk(3);
        ID.run();

        // std::cout<<str[ID.gettype()]<<std::endl;
        // reg.printdata();

        // if (cnt%6==2)
        // {
        //     IF.putback();
        //     IF.putwclk(5);
        // }
        if (isJump(ID.gettype()))
        {
            IF.reset();
            IF.putwclk(3);  //1+1+1 without SL
            // IF.putwclk(6);
        }
        if (isSL(ID.gettype()))
        {
            IF.reset();
            IF.putwclk(6);  //1+1+3+1
            // IF.putwclk(6);
        }
        EXE.init(ID);
        IF.run();
        // std::cout<<reg.getpc()<<std::endl;
        ID.init(IF);
    } while (!WB.isEnd());
    std::cout<<cnt<<std::endl;
    std::cout<<reg.output()<<std::endl;
    return 0;
}