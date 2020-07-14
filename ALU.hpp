#ifndef __ALU__
#define __ALU__

#include"RISC-V.h"
#include"executor.hpp"

class ALU
{
    private:
        Executor opt;
    public:
        void reset()
        {
            opt.reset();
        }
        void run()
        {
            opt.run();
        }   
};

#endif