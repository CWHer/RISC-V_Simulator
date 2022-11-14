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
        PIPELINE
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
        do
        {
            // std::cout << "cycle: " << std::dec << total_cycles
            //           << ", pc: " << std::hex << reg_file.getPC() << std::endl;
            total_cycles++;

            WB.run();
            // WB.printInst();
            MEM.run(), WB.init(MEM);
            // MEM.printInst();
            EXE.run(), MEM.init(EXE);
            auto next_pc = ID.getInstAddr();
            if (isRegDest(EXE.getType()))
            {
                static const int STALL_CYCLES = 2;
                IF.putLock(STALL_CYCLES);
                ID.putLock(STALL_CYCLES);
            }
            if (EXE.checkBranchPred(next_pc))
            {
                IF.reset(), ID.reset();
                // std::cout << "==========Reset==========" << std::endl;
            }
            // EXE.printInst();
            ID.run(), EXE.init(ID);
            // ID.printInst();
            IF.run(), ID.init(IF);
            // IF.printInst();

            // reg_file.printRegfile();
        } while (!WB.isDone());
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
        case PIPELINE:
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
