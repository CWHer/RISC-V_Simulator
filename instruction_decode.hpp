#ifndef __INSTRUCTION_DECODE_HPP__
#define __INSTRUCTION_DECODE_HPP__

#include "instruction.hpp"
#include "memory.hpp"
#include "register_file.hpp"
#include "predictor.hpp"
#include "instruction_fetch.hpp"

class InstructionDecode
{
    friend class Execute;

private:
    Instruction inst;
    bool is_done;
    int wait_cycles;

public:
    InstructionDecode() : is_done(false), wait_cycles(0) {}

    void reset()
    {
        inst.type = EMPTY;
        wait_cycles = 0;
    }

    void init(InstructionFetch &IF)
    {
        // clang-format off
        if (this->isLock()) return;
        this->reset();
        if (IF.isLock()) return;
        // clang-format on

        inst = IF.inst;
        is_done = IF.is_done;
    }

    void run()
    {
        if (!is_done)
        {
            // clang-format off
            if (--wait_cycles > 0) return;
            // clang-format on
            inst.decode();
        }
    }

    void putLock(int cycles)
    {
        wait_cycles = cycles;
    }

    bool isLock()
    {
        return wait_cycles > 0;
    }

    void printInst()
    {
        std::cout << "[ID] Inst addr" << std::hex << inst.addr
                  << ", Inst type: " << INST_STRING[inst.type] << std::endl;
    }
};

#endif