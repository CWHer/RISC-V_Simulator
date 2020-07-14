#ifndef __REORDERBUFFER__
#define __REORDERBUFFER__

#include"RISC-V.h"
#include<queue>

class ReorderBuffer
{
    private:
        struct ROBnode
        {
            Instructiontypes type;
            unsigned pc;
            unsigned dst;   //destination in reg
            unsigned val;
            bool isReady;
            ROBnode() 
            {
                type=EMPTY;
                dst=val=isReady=0;
            }
        };
        std::queue<ROBnode> Q;
    public:


};

#endif