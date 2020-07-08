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
    public:
        void init(MemoryAccess &MEM)
        {
            reg=MEM.reg;
            mem=MEM.mem;
            exe=MEM.exe;
        }
        void run()
        {
            exe.write_back(reg);
        }
};

#endif