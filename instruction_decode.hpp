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
    RegisterFile *reg_file;

    Instruction inst;
    bool is_done;
    int wait_cycles;

public:
    InstructionDecode(RegisterFile *reg_file)
        : reg_file(reg_file), is_done(false), wait_cycles(0) {}

    void reset()
    {
        inst.type = EMPTY;
        // NOTE: HACK: deactivate inst.decode()
        inst.inst_bytes = 0;
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
            inst.readReg(reg_file);
        }
    }

    void putLock(int cycles)
    {
        if (!is_done)
            wait_cycles = cycles;
    }

    bool isLock()
    {
        return wait_cycles > 0;
    }

    unsigned getInstAddr() { return inst.addr; }

    void printInst()
    {
        std::cout << "[ID] Inst addr: " << std::hex << inst.addr
                  << ", Inst type: " << INST_STRING[inst.type] << std::endl;
    }
};

#endif