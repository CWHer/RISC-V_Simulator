#ifndef __ALU_HPP__
#define __ALU_HPP__

#include "RISC-V.h"
#include "executable.hpp"

// Arithmetic Logic Unit
class ALUnit
{
    friend class CommonDataBus;

private:
    ExecWarp *executable;
    int busy_cycles;

public:
    ALUnit() : executable(nullptr),
               busy_cycles(0) {}

    void send(ResEntry *entry)
    {
        executable = new ExecWarp();
        executable->init(entry);
        busy_cycles = 0;
    }

    void reset()
    {
        executable = nullptr;
        busy_cycles = 0;
    }

    void start(int cycles) { busy_cycles = cycles; }

    bool isBusy() { return busy_cycles > 0; }

    void run()
    {
        if (--busy_cycles > 0)
            return;
        executable->exec();
    }

    bool empty()
    {
        return executable == nullptr;
    }

    void printEntry()
    {
        if (!empty())
            std::cout << "ALU: "
                      << INST_STRING[executable->getType()] << std::endl;
    }
};

#endif