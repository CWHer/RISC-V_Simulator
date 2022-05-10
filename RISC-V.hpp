#include "RISC-V.h"
#include "register.hpp"
#include "memory.hpp"
#include "predictor.hpp"
#include "instruction_fetch.hpp"
#include "instruction_decode.hpp"
#include "execute.hpp"
#include "memory_access.hpp"
#include "writeback.hpp"

class RISC_V // mode  0(default):serial   1:parallel
{
private:
    Register reg;
    Memory *mem;
    Predictor prd;
    InstructionFetch IF;
    InstructionDecode ID;
    Execute EXE;
    MemoryAccess MEM;
    WriteBack WB;
    int mode;
    void run_parallel()
    {
        bool MEM2WB, isPB, isStall; // MEM->WB? putback? curisstall?
        mem->init_read();
        IF.init(mem, &reg);
        do
        {
            ++clkcnt, isPB = isStall = 0;
            WB.run();
            MEM.run();
            MEM2WB = MEM.gettype() != EMPTY;
            WB.init(MEM);
            if (!MEM.isLock() && MEM.gettype() != EMPTY)
                MEM.forwarding(EXE); // MEM->EXE
            EXE.run();
            if (!EXE.check()) // put back pipeline to last clk
            {                 // jump&incorrect pred
                reg.prevpc();
                EXE.putback(ID);
                EXE.reset();
                ID.setJump();
                isPB = 1;
            }
            if (EXE.willJump())
                EXE.forwarding(IF); // EXE->IF
            EXE.update(&prd);       // update predictor
            MEM.init(EXE);
            if (EXE.gettype() != EMPTY || EXE.isEnd())
            {
                if (!isSL(EXE.gettype())) // skip MEM if without SL
                {
                    if (MEM2WB)      // structural hazard
                        isStall = 1; // MEM->WB EXE->WB at same cycle
                    else             // stall 1 clk
                        WB.init(MEM);
                    MEM.reset();
                }
                else
                    MEM.putwclk(3);
            }
            if (!isPB && !isStall)
                ID.run();
            if (ID.willJump()) // b-type:1+1+1/EXE->IF-1
                IF.reset(), IF.putwclk(2);
            if (isSL(ID.gettype())) // s&L:3+1/MEM->EXE-1
                IF.reset(), IF.putwclk(3);
            if (!isStall)
                EXE.init(ID);
            if (!isStall)
                IF.run();
            if (!isStall)
                ID.init(IF);
        } while (!WB.isEnd());
    }
    void run_serial()
    {
        mem->init_read();
        IF.init(mem, &reg);
        do
        {
            IF.run(), ++clkcnt;
            ID.init(IF);
            ID.run(), ++clkcnt;
            EXE.init(ID);
            EXE.run();
            if (isSL(EXE.gettype()))
                clkcnt += 3;
            MEM.init(EXE);
            MEM.run(), ++clkcnt;
            WB.init(MEM);
            WB.run(), ++clkcnt;
        } while (!WB.isEnd());
    }
    int clkcnt; // clock cycle cnt
public:
    RISC_V(Memory *_mem, int _mode = 0)
        : clkcnt(0), mem(_mem), mode(_mode), ID(&prd) {}
    void run()
    {
        if (mode == 0)
            run_serial();
        else
            run_parallel();
    }
    unsigned output()
    {
        return reg.output();
    }
    int clktimes()
    {
        return clkcnt;
    }
    void prdrate()
    {
        int num = prd.tot - EXE.tot();
        printf("%d/%d %.2lf%\n", num, prd.tot, num * 100.0 / prd.tot);
    }
};
