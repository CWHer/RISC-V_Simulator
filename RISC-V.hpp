#include"RISC-V.h"
#include"register.hpp"
#include"memory.hpp"
#include"instructionfetch.hpp"
#include"instructiondecode.hpp"
#include"execute.hpp"
#include"memoryaccess.hpp"
#include"writeback.hpp"

class RISC_V        //mode  0(default):serial   1:parallel
{
    private:
        Register reg;
        Memory *mem;
        InstructionFetch IF;
        InstructionDecode ID;
        Execute EXE;
        MemoryAccess MEM;
        WriteBack WB;
        int mode;
        void run_parallel()
        {
            mem->init_read();
            IF.init(mem,&reg);
            do {
                ++cnt;
                WB.run();
                MEM.run();
                WB.init(MEM);
                EXE.run();
                MEM.init(EXE);
                if (EXE.gettype()!=EMPTY||EXE.isEnd())  
                {
                    if (!isSL(EXE.gettype()))   //ignore MEM if without SL
                        WB.init(MEM),MEM.reset();
                    else
                        MEM.putwclk(3);
                }
                ID.run();
                if (isJump(ID.gettype()))
                {
                    IF.reset();
                    IF.putwclk(3);  //1+1+1 without SL
                }
                if (isSL(ID.gettype()))
                {
                    IF.reset();
                    IF.putwclk(6);  //1+1+3+1
                }
                EXE.init(ID);
                IF.run();
                ID.init(IF);
            } while (!WB.isEnd());
        }
        void run_serial()
        {
            mem->init_read();
            IF.init(mem,&reg);
            do {
                ++cnt;
                IF.run();
                ID.init(IF);
                ID.run();
                EXE.init(ID);
                EXE.run();
                MEM.init(EXE);
                MEM.run();
                WB.init(MEM);
                WB.run();
            } while (!WB.isEnd());
        }
        //debug
        int cnt;
    public:
        RISC_V(Memory *_mem,int _mode=0):cnt(0),mem(_mem),mode(_mode) {}
        void run()
        {
            if (mode==0)
                run_serial();
            else
                run_parallel();
        }
        unsigned output()
        {
            return reg.output();
        }
        //debug
        int exetimes()
        {
            return cnt;
        }
};
