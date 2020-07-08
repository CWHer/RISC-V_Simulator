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
    public:
        void init(Execute &EXE)
        {
            reg=EXE.reg;
            mem=EXE.mem;
            exe=EXE.exe;
        }
        void run()
        {
            exe.memory_access(mem,reg);
        }
};

#endif