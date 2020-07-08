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
        bool isend;
        int wait_clk;
    public:
        InstructionDecode():wait_clk(0) {}
        void init(InstructionFetch &IF)
        {
            if (isLock()) return;
            reset();
            if (IF.isLock())
            {
                reset();
                return;
            }
            opt=IF.opt;
            reg=IF.reg;
            mem=IF.mem;
            isend=IF.isend;
        }
        void reset()    //reset to EMPTY
        {
            opt.init();
        }
        void run()
        {
            if (wait_clk>0)
            {
                --wait_clk;
                if (wait_clk) return;
            }
            if (isend) return;
            opt.decode();
        }
        void putwclk(int clk)  //put wait clk
        {
            wait_clk+=clk;
        }
        bool isEnd()
        {
            return isend;
        }
        Instructiontypes gettype()
        {
            return opt.gettype();
        }
        bool isLock()
        {
            return wait_clk>0;
        }
};

#endif