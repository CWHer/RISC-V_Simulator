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

    ExecWrapper executable;
    bool is_done;
    int wait_cycles;
    int predict_error_cnt;
    ForwardCapsule forward_capsule;

public:
    Execute(RegisterFile *reg_file)
        : reg_file(reg_file), is_done(false),
          wait_cycles(0), predict_error_cnt(0) {}

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

    // bool willJump() // can only use after run!
    // {
    //     return executable.temp_resultpc != 0;
    // }

    // bool checkBranchPred()
    // {
    //     if ((!executable.opt.willJump()) ^ (executable.temp_resultpc == 0)) // inconsistent
    //     {
    //         ++predict_error_cnt;
    //         if (executable.temp_resultpc != 0)
    //             return 0;
    //     }
    //     return 1;
    // }

    // void update(Predictor *prd) // feedback predictor
    // {
    //     InstructionTypes type = getType();
    //     if (isJumpInst(type) != 1)
    //         return;
    //     prd->update(executable.gettype(), willJump() ? -1 : 1);
    //     prd->push(executable.gettype(), willJump());
    // }

    int errorNum()
    {
        return predict_error_cnt;
    }

    void printInst()
    {
        std::cout << "[EXE] Inst addr" << std::hex << executable.instAddr()
                  << ", Inst type: " << INST_STRING[executable.getType()] << std::endl;
    }
};

#endif