#include "register.hpp"
#include "memory.hpp"
#include "RISC-V.h"
#include "instruction_fetch.hpp"
#include "instruction_decode.hpp"
#include "execute.hpp"
#include "memory_access.hpp"
#include "writeback.hpp"

Predictor prd;
Register reg;
Memory mem;
InstructionFetch IF;
InstructionDecode ID(&prd);
Execute EXE;
MemoryAccess MEM;
WriteBack WB;

int main()
{

    int cnt = 0;
    // freopen("out","w",stdout);
    mem.init_read();
    IF.init(&mem, &reg);
    // freopen("in","r",stdin);
    while (!WB.isEnd())
    {
        // std::cout<<reg.output()<<std::endl;
        IF.run(), ++cnt;
        ID.init(IF);
        ID.run(), ++cnt;

        // std::cout<<++cnt<<' '<<str[ID.gettype()]<<std::endl;
        // std::cout<<reg.getpc()<<std::endl;

        EXE.init(ID);
        EXE.run();
        if (isSL(EXE.gettype()))
            cnt += 3;
        MEM.init(EXE);
        MEM.run(), ++cnt;
        WB.init(MEM);
        WB.run(), ++cnt;

        // reg.printdata();
    }
    std::cout << cnt << std::endl;
    std::cout << reg.output() << std::endl;
    return 0;
}