#ifndef __EXECUTE_HPP__
#define __EXECUTE_HPP__

#include "instruction.hpp"
#include "memory.hpp"
#include "register_file.hpp"
#include "executable.hpp"
#include "predictor.hpp"
#include "instruction_fetch.hpp"
#include "instruction_decode.hpp"

class Execute
{
    friend class MemoryAccess;

private:
    RegisterFile *reg_file;
    PatternHistoryTable *predictor;

    ExecWrapper executable;
    bool is_done;
    int wait_cycles;
    int predict_error_cnt;
    ForwardCapsule forward_capsule;

public:
    Execute(RegisterFile *reg_file,
            PatternHistoryTable *predictor)
        : reg_file(reg_file), predictor(predictor),
          is_done(false), wait_cycles(0), predict_error_cnt(0) {}

    void reset()
    {
        executable.reset();
        forward_capsule.reset();
        wait_cycles = 0;
    }

    void init(InstructionDecode &ID)
    {
        // clang-format off
        if (this->isLock()) return;
        this->reset();
        if (ID.isLock()) return;
        // clang-format on

        executable.init(ID.inst);
        is_done = ID.is_done;
    }

    void run()
    {
        if (!is_done)
        {
            // clang-format off
            if (--wait_cycles > 0) return;
            // clang-format on
            executable.exec(reg_file, forward_capsule);
            forward_capsule.reset();
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

    InstructionTypes getType()
    {
        return executable.getType();
    }

    bool checkBranchPred(unsigned next_pc)
    {
        if (isJumpInst(getType()) != NOT_JUMP)
        {
            static const int INST_LEN = 4;
            bool is_jumped = executable.pc_result !=
                             executable.instAddr() + INST_LEN;
            predictor->update(executable.instAddr(), is_jumped ? -1 : 1);
            if (next_pc != executable.pc_result)
            {
                // catastrophic condition: branch mis-predicted
                predict_error_cnt++;
                reg_file->setPC(executable.pc_result);
                return true;
            }
        }
        return false;
    }

    int errorNum()
    {
        return predict_error_cnt;
    }

    void printInst()
    {
        std::cout << "[EXE] Inst addr: " << std::hex << executable.instAddr()
                  << ", Inst type: " << INST_STRING[executable.getType()] << std::endl;
    }
};

#endif