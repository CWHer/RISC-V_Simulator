#ifndef __MEMORY_ACCESS_HPP__
#define __MEMORY_ACCESS_HPP__

#include "instruction.hpp"
#include "memory.hpp"
#include "register_file.hpp"
#include "executable.hpp"
#include "execute.hpp"

class MemoryAccess
{
    friend class WriteBack;

private:
    Memory *memory;

    ExecWrapper executable;
    bool is_done;
    int wait_cycles;
    ForwardCapsule forward_capsule;

public:
    MemoryAccess(Memory *mem)
        : memory(mem), is_done(false), wait_cycles(0) {}

    void reset()
    {
        executable.reset();
        forward_capsule.reset();
        wait_cycles = 0;
    }

    void init(Execute &EXE)
    {
        // clang-format off
        if (this->isLock()) return;
        this->reset();
        if (EXE.isLock()) return;
        // clang-format on

        executable = EXE.executable;
        is_done = EXE.is_done;
    }

    void run()
    {
        if (!is_done)
        {
            // clang-format off
            if (--wait_cycles > 0) return;
            // clang-format on
        }
        executable.memoryAccess(memory);
        if (isRegDest(executable.getType()))
            forward_capsule = executable.makeForward();
    }

    void forward(Execute &EXE)
    {
        EXE.forward_capsule = forward_capsule;
        forward_capsule.reset();
    }

    void putLock(int cycles)
    {
        wait_cycles = cycles;
    }

    bool isLock()
    {
        return wait_cycles > 0;
    }

    InstructionTypes getType()
    {
        return executable.getType();
    }

    void printInst()
    {
        std::cout << "[MEM] Inst addr" << std::hex << executable.instAddr()
                  << ", Inst type: " << INST_STRING[executable.getType()] << std::endl;
    }
};

#endif