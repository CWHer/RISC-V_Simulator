#ifndef __WRITE_BACK_HPP__
#define __WRITE_BACK_HPP__

#include "RISC-V.h"
#include "instruction.hpp"
#include "memory.hpp"
#include "register_file.hpp"
#include "executable.hpp"
#include "memory_access.hpp"

class WriteBack
{
private:
    RegisterFile *reg_file;

    ExecWrapper executable;
    bool is_done;

    unsigned commited_inst;

public:
    WriteBack(RegisterFile *reg_file)
        : reg_file(reg_file), is_done(false), commited_inst(0) {}

    void init(MemoryAccess &MEM)
    {
        this->reset();
        if (MEM.isLock())
            return;
        executable = MEM.executable;
        is_done = MEM.is_done;
    }
    void reset() { executable.reset(); }

    void run()
    {
        if (!is_done)
        {
            executable.writeBack(reg_file);
            commited_inst++;
        }
    }

    void printInst()
    {
        std::cout << "[WB] Seq: " << commited_inst
                  << ", Inst addr" << std::hex << executable.instAddr()
                  << ", Inst type: " << INST_STRING[executable.getType()] << std::endl;
    }

    bool isDone()
    {
        return is_done;
    }
};

#endif