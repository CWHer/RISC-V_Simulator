#include"instruction.hpp"
#include"memory.hpp"
#include"register.hpp"

#ifndef _INSTRUCTIONFETCH_
#define _INSTRUCTIONFETCH_

class InstructionFetch
{
    friend class InstructionDecode;
    private:
        Instruction opt;
        Register *reg;
        Memory *mem;
    public:
        void init(Memory *_mem,Register *_reg)
        {
            mem=_mem;
            reg=_reg;
        }
        bool run()
        {
            return opt.fetch(mem,reg);
        }
};

#endif