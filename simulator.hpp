#include "RISC-V.h"
#include "register_file.hpp"
#include "memory.hpp"
#include "predictor.hpp"
#include "instruction_fetch.hpp"
#include "instruction_decode.hpp"
#include "executable.hpp"
#include "memory_access.hpp"
#include "write_back.hpp"

class Simulator
{
public:
    enum Mode
    {
        SERIAL,
        PARALLEL
    };

private:
    int total_cycles;
    Mode mode;

    RegisterFile reg_file;
    Memory *memory;
    PatternHistoryTable predictor;

    InstructionFetch IF;
    InstructionDecode ID;
    Execute EXE;
    MemoryAccess MEM;
    WriteBack WB;

private:
    void runPipeline()
    {
        // bool MEM2WB, isPB, isStall; // MEM->WB? putback? curisstall?
        // memory->init_read();
        // IF.init(memory, &reg_file);
        // do
        // {
        //     // TODO
        //     ++total_cycles, isPB = isStall = 0;
        //     WB.run();
        //     MEM.run();
        //     MEM2WB = MEM.getType() != EMPTY;
        //     WB.init(MEM);
        //     if (!MEM.isLock() && MEM.getType() != EMPTY)
        //         MEM.forward(EXE); // MEM->EXE
        //     EXE.run();
        //     if (!EXE.checkBranchPred()) // put back pipeline to last clk
        //     {                           // jump&incorrect pred
        //         reg_file.prevpc();
        //         EXE.putback(ID);
        //         EXE.reset();
        //         ID.setJump();
        //         isPB = 1;
        //     }
        //     if (EXE.willJump())
        //         EXE.forwarding(IF); // EXE->IF
        //     EXE.update(&prd);       // update predictor
        //     MEM.init(EXE);
        //     if (EXE.getType() != EMPTY || EXE.isEnd())
        //     {
        //         if (!isMemoryInst(EXE.getType())) // skip MEM if without SL
        //         {
        //             if (MEM2WB)      // structural hazard
        //                 isStall = 1; // MEM->WB EXE->WB at same cycle
        //             else             // stall 1 clk
        //                 WB.init(MEM);
        //             MEM.reset();
        //         }
        //         else
        //             MEM.putwclk(3);
        //     }
        //     if (!isPB && !isStall)
        //         ID.run();
        //     if (ID.willJump()) // b-type:1+1+1/EXE->IF-1
        //         IF.reset(), IF.putLock(2);
        //     if (isMemoryInst(ID.getType())) // s&L:3+1/MEM->EXE-1
        //         IF.reset(), IF.putLock(3);
        //     if (!isStall)
        //         EXE.init(ID);
        //     if (!isStall)
        //         IF.run();
        //     if (!isStall)
        //         ID.init(IF);
        // } while (!WB.isEnd());
    }

    void runSerial()
    {
        do
        {
            // std::cout << "cycle: " << std::dec << total_cycles
            //           << ", pc: " << std::hex << reg_file.getPC() << std::endl;
            total_cycles += 5;
            IF.run();
            // IF.printInst();
            ID.init(IF), ID.run();
            // ID.printInst();
            EXE.init(ID), EXE.run();
            EXE.checkBranchPred(reg_file.getPC());
            // EXE.printInst();
            MEM.init(EXE), MEM.run();
            // MEM.printInst();
            WB.init(MEM), WB.run();
            // WB.printInst();
            // reg_file.printRegfile();
        } while (!WB.isDone());
    }

public:
    Simulator(Memory *mem)
        : total_cycles(0), memory(mem),
          IF(mem, &reg_file, &predictor), ID(),
          EXE(&reg_file, &predictor), MEM(mem), WB(&reg_file) {}

    void setMode(Mode mode)
    {
        this->mode = mode;
    }

    void run()
    {
        switch (mode)
        {
        case SERIAL:
            runSerial();
            break;
        case PARALLEL:
            runPipeline();
            break;
        }
    }

    unsigned getResult()
    {
        return reg_file.getResult();
    }

    void printStatics()
    {
        std::cout << "Total cycles: " << std::dec << total_cycles << std::endl;
        int total_predict = predictor.getTotalPredict();
        int correct_predict = total_predict - EXE.errorNum();
        std::cout << "Predictor accuracy: " << correct_predict << "/" << total_predict << " ("
                  << (double)correct_predict / total_predict * 100 << "%)" << std::endl;
    }
};
