#include"instruction.hpp"
#include"memory.hpp"
#include"register.hpp"
#include"executor.hpp"
#include"memoryaccess.hpp"

#ifndef _WRITEBACK_
#define _WRITEBACK_

class WriteBack
{
    private:
        Register *reg;
        Memory *mem;
        Executor exe;
        bool isend;
    public:
        void init(MemoryAccess &MEM)
        {
            reset();
            if (MEM.isLock())
            {
                reset();
                return;
            }
            reg=MEM.reg;
            mem=MEM.mem;
            exe=MEM.exe;
            isend=MEM.isend;
        }
        void reset()    //reset to EMPTY
        {
            Instruction opt;
            exe.init(opt);
        }
        void run()
        {
            if (isend) return;
            exe.write_back(reg);
        }
        bool isEnd()
        {
            return isend;
        }
        Instructiontypes gettype()
        {
            return exe.gettype();
        }
};

#endif