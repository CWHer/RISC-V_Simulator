#include"instruction.hpp"
#include"memory.hpp"
#include"register.hpp"
#include"executor.hpp"
#include"instructiondecode.hpp"

#ifndef _EXECUTE_
#define _EXECUTE_

class Execute
{
    friend class MemoryAccess;
    private:
        Register *reg;
        Memory *mem;
        Executor exe;
        bool isend;
        int wait_clk;
    public:
        Execute():wait_clk(0) {}
        void init(InstructionDecode &ID)
        {
            if (isLock()) return;
            reset();
            if (ID.isLock())
            {
                reset();
                return;
            }
            reg=ID.reg;
            mem=ID.mem;
            exe.init(ID.opt);
            isend=ID.isend;
        }
        void reset()    //reset to EMPTY
        {
            Instruction opt;
            exe.init(opt);
        }
        void run()
        {
            if (wait_clk>0) 
            {
                --wait_clk;
                if (wait_clk) return;
            }
            if (isend) return;
            exe.run(reg);
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
            return exe.gettype();
        }
        bool isLock()
        {
            return wait_clk>0;
        }
};

#endif