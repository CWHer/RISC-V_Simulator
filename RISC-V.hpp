#include"RISC-V.h"
#include"instruction.hpp"
#include"execute.hpp"
#include"register.hpp"
#include"memory.hpp"

class RISC_V
{
    private:
        Register reg;
        Instruction opt;
        Memory *mem;
        int mode;
        void run_parallel()
        {
            
        }
        void run_serial()
        {
            mem->init_read();
            opt.fetch(mem,&reg);
            while (!opt.isEnd())
            {
                ++cnt; 
                opt.decode();
                Execute exe(&opt,&reg,mem);
                exe.run();
                exe.memory_access();
                exe.write_back();
                opt.fetch(mem,&reg);
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
