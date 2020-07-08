#include"RISC-V.h"
#include"register.hpp"
#include"memory.hpp"
#include"instructionfetch.hpp"
#include"instructiondecode.hpp"
#include"execute.hpp"
#include"memoryaccess.hpp"
#include"writeback.hpp"

class RISC_V
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
            // exe.write_back();
            // exe.memory_access();
            // exe.run();
            // opt.decode();
            // opt.fetch();
        }
        void run_serial()
        {
            mem->init_read();
            IF.init(mem,&reg);
            while (IF.run())
            {
                ++cnt;
                ID.init(IF);
                ID.run();
                EXE.init(ID);
                EXE.run();
                MEM.init(EXE);
                MEM.run();
                WB.init(MEM);
                WB.run();
            }
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
