#include"instruction.hpp"
#include"memory.hpp"
#include"register.hpp"
#include"executor.hpp"
#include"execute.hpp"

#ifndef _MEMORYACCESS_
#define _MEMORYACCESS_

class MemoryAccess
{
    friend class WriteBack;
    private:
        Register *reg;
        Memory *mem;
        Executor exe;
        bool isend;
        int wait_clk;
    public:
        void init(Execute &EXE)
        {
            if (isLock()) return;
            reset();
            if (EXE.isLock())
            {
                reset();
                return;
            }
            reg=EXE.reg;
            mem=EXE.mem;
            exe=EXE.exe;
            isend=EXE.isend;
        }
        void reset()    //reset to EMPTY
        {
            Instruction opt;
            exe.init(opt);
        }
        void run()
        {
            if (isend) return;
            if (wait_clk>0)
            {
                --wait_clk;
                if (wait_clk) return;
            }
            // if (isSL(exe.gettype())) 
            // putwclk(2);    //memaccess needs 3 clk
            exe.memory_access(mem,reg);
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