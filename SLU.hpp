#ifndef __SLU__
#define __SLU__

#include"RISC-V.h"
#include"executor.hpp"

class SLU
{
    private:
        Executor opt;
        int wait_clk;
    public:
        SLU():wait_clk(0) {}
        void reset()
        {
            opt.reset();
            wait_clk=0;
        }
        void putwclk(int clk)
        {
            wait_clk+=clk;
        }
        bool isLock()
        {
            return wait_clk>0;
        }
        void run()
        {
            opt.run();
        }
};

#endif 