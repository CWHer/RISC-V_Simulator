#ifndef __INSTRUCTION_FETCH_HPP__
#define __INSTRUCTION_FETCH_HPP__

#include "instruction.hpp"
#include "memory.hpp"
#include "register_file.hpp"

class InstructionFetch
{
    friend class InstructionDecode;

private:
    Memory *memory;
    RegisterFile *reg_file;
    PatternHistoryTable *predictor;

    Instruction inst;
    bool is_done;
    int wait_cycles;

public:
    InstructionFetch(Memory *mem, RegisterFile *reg_file,
                     PatternHistoryTable *predictor)
        : memory(mem), reg_file(reg_file),
          predictor(predictor), is_done(false), wait_cycles(0) {}

    void reset()
    {
        inst.type = EMPTY;
        wait_cycles = 0;
    }

    void run()
    {
        if (!is_done)
        {
            // clang-format off
            if (--wait_cycles > 0) return;
            // clang-format on

            is_done = inst.fetch(memory, reg_file);
            if (!is_done)
            {
                // HACK: ignore JALR
                if (inst.fastDecode() &&
                    inst.basic_type != I &&
                    predictor->predict(inst.addr))
                    reg_file->setPC(inst.addr + inst.imm);
                else
                    reg_file->nextPC();
            }
        }
    }

    void putLock(int cycles)
    {
        wait_cycles = cycles;
    }

    bool isDone()
    {
        return is_done;
    }

    bool isLock()
    {
        return wait_cycles > 0;
    }

    void printInst()
    {
        std::cout << "[IF] Inst addr: " << std::hex << inst.addr
                  << ", Inst type: " << INST_STRING[inst.type] << std::endl;
    }
};

#endif