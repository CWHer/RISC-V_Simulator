#ifndef __ISSUE_HPP__
#define __ISSUE_HPP__

#include "RISC-V.h"
#include "instruction.hpp"
#include "register_file.hpp"
#include "memory.hpp"
#include "reservation_station.hpp"
#include "reorder_buffer.hpp"
#include "predictor.hpp"

class Issue
{
private:
    Memory *memory;
    RegisterFile *reg_file;
    PatternHistoryTable *predictor;
    bool is_done;

public:
    Issue(Memory *mem, RegisterFile *reg_file,
          PatternHistoryTable *predictor)
        : memory(mem), reg_file(reg_file),
          predictor(predictor), is_done(false) {}

    void issueInst(ReservationStation *res, ReorderBuffer *rob)
    {
        Instruction inst;
        if (!inst.fetch(memory, reg_file))
        {
            is_done = true;
            return;
        }
        inst.decode();

        // clang-format off
        if (rob->full() || res->isFull(inst.type)) return;
        // clang-format on

        // NOTE: branch prediction
        if (inst.type != JALR &&
            isJumpInst(inst.type) != NOT_JUMP)
        {
            if (inst.type == JAL ||
                predictor->predict(inst.addr))
                reg_file->setPC(inst.addr + inst.imm);
            else
                reg_file->nextPC();
        }
        else
            reg_file->nextPC();

        auto dest = rob->push(inst);
        res->push(inst, dest);
        if (isMemoryInst(inst.type) != MEM_STORE &&
            isJumpInst(inst.type) != CONDITIONAL_JUMP)
            reg_file->setDep(inst.rd, dest);
    }

    void reset() { is_done = false; }

    bool isDone() { return is_done; }
};

#endif