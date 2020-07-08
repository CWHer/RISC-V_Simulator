#include"instruction.hpp"
#include"memory.hpp"
#include"register.hpp"
#include"instructionfetch.hpp"

#ifndef _INSTRUCTIONDECODE_
#define _INSTRUCTIONDECODE_

class InstructionDecode
{
    friend class Execute;
    private:
        Instruction opt;
        Register *reg;
        Memory *mem;
    public:
        void init(InstructionFetch &IF)
        {
            opt=IF.opt;
            reg=IF.reg;
            mem=IF.mem;
        }
        void run()
        {
            opt.decode();
        }
        //debug
        int gettype()
        {
            return opt.gettype();
        }
};

#endif