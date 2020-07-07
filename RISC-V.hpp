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
        //debug
        int cnt;
    public:
        RISC_V(Memory *_mem):cnt(0),mem(_mem) {}
        void run()
        {
            mem->init_read();
            while (!opt.empty())
            {
                ++cnt;
                opt.fetch(mem,&reg);
                opt.decode();
                Execute exe(&opt,&reg,mem);
                exe.run();
                exe.memory_access();
                exe.write_back();
            }
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
