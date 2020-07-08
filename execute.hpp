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
    public:
        void init(InstructionDecode &ID)
        {
            reg=ID.reg;
            mem=ID.mem;
            exe.init(ID.opt);
        }
        void run()
        {
            exe.run(reg);
        }
};

#endif