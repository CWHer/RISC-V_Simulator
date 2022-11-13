#ifndef __SLU_HPP__
#define __SLU_HPP__

#include "RISC-V.h"
#include "executable.hpp"
#include "memory.hpp"

class SLUnit
{
    friend class CommonDataBus;

private:
    Memory *memory;
    ExecWrapper *executable;
    int busy_cycles;

public:
    SLUnit(Memory *mem)
        : memory(mem), executable(nullptr),
          busy_cycles(0) {}

    void send(ResEntry *entry)
    {
        executable = new ExecWrapper();
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
        executable->memoryAccess(memory);
    }

    bool empty()
    {
        return executable == nullptr;
    }

    void printEntry()
    {
        if (!empty())
            std::cout << "SLU: "
                      << INST_STRING[executable->getType()] << std::endl;
    }
};

#endif