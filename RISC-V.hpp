#include"RISC-V.h"
#include"register.hpp"
#include"memory.hpp"
#include"predictor.hpp"
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
        Predictor prd;
        InstructionFetch IF;
        InstructionDecode ID;
        Execute EXE;
        MemoryAccess MEM;
        WriteBack WB;
        void run_serial()
        {
            mem->init_read();
            IF.init(mem,&reg);
            do {
                IF.run(),++clkcnt;
                ID.init(IF);
                ID.run(),++clkcnt;
                EXE.init(ID);
                EXE.run();
                if (isSL(EXE.gettype())) clkcnt+=3;
                MEM.init(EXE);
                MEM.run(),++clkcnt;
                WB.init(MEM);
                WB.run(),++clkcnt;
            } while (!WB.isEnd());
        }
        //debug
        int clkcnt,wcnt;    //clockcnt wrongcnt
    public:
        RISC_V(Memory *_mem,int _mode=0)
            :clkcnt(0),wcnt(0),mem(_mem),mode(_mode),ID(&prd),WB(_mode) {}
        void run()
        {

        }
        unsigned output()
        {
            return reg.output();
        }
        //debug
        int clktimes()
        {
            return clkcnt;
        }
        void prdrate()
        {
            int rcnt=prd.tot-wcnt;
            printf("%d/%d %.2lf%\n",rcnt,prd.tot,rcnt*100.0/prd.tot);
        }
};
