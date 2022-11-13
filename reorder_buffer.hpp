#ifndef __REORDER_BUFFER_HPP__
#define __REORDER_BUFFER_HPP__

#include "RISC-V.h"
#include "memory.hpp"
#include "register_file.hpp"
#include "predictor.hpp"
#include "instruction.hpp"
#include "executable.hpp"

class ReorderBuffer
{
    friend class CommonDataBus;

private:
    Memory *memory;
    RegisterFile *reg_file;
    PatternHistoryTable *predictor;

    unsigned predict_error_cnt;
    unsigned commited_inst;
    unsigned interrupt_index;

    static const int MAX_SIZE = 10;
    std::deque<ROBEntry> inst_queue;

public:
    ReorderBuffer(Memory *mem, RegisterFile *reg_file,
                  PatternHistoryTable *predictor, unsigned interrupt_index = 0)
        : memory(mem), reg_file(reg_file),
          predictor(predictor), predict_error_cnt(0),
          commited_inst(0), interrupt_index(interrupt_index) {}

    bool is_stalled()
    {
        // HACK:
        //  1. JALR in ROB
        //  2. S-type in SLU (WAW, WAR hazards)
        for (const auto &inst : inst_queue)
            if (inst.inst_type == JALR ||
                isMemoryInst(inst.inst_type) == MEM_STORE)
                return true;
        return false;
    }

    bool full() { return inst_queue.size() == MAX_SIZE; }

    bool empty() { return inst_queue.empty(); }

    bool isReady()
    {
        return inst_queue.front().is_done;
    }
    void reset() { inst_queue.clear(); }

    ROBEntry *push(Instruction inst)
    {
        ROBEntry entry;
        entry.dest_reg = inst.rd;
        entry.inst_addr = inst.addr;
        entry.inst_type = inst.type;
        inst_queue.push_back(entry);

        return &inst_queue.back();
    }

    void update(ExecWarp *executable)
    {
        executable->entry.dest->is_done = true;
    }

    // return true if branch is mis-predicted
    bool commitInst()
    {
        // clang-format off
        auto getExec = [](const ROBEntry &entry) {
            return reinterpret_cast<ExecWarp *>(entry.executable);
        };

        ROBEntry entry = inst_queue.front();
        if (!entry.is_done) return false;
        ExecWarp *executable = getExec(entry);
        // clang-format on

        if (commited_inst == interrupt_index)
        {
            // interrupt demo
            std::cout << "Interrupt!" << std::endl;
            std::cout << "Inst addr: " << std::hex << executable->entry.inst_addr
                      << ", Inst type: " << INST_STRING[executable->getType()] << std::endl;
        }

        commited_inst++;
        executable->writeBack(entry.dest_reg, memory, reg_file);
        if (reg_file->getDep(entry.dest_reg) == &inst_queue.front())
            reg_file->setDep(entry.dest_reg, nullptr);
        inst_queue.pop_front();

        // std::cout << "Seq: " << std::dec << commited_inst
        //           << ", Inst addr: " << std::hex << executable->entry.inst_addr
        //           << ", Inst type: " << INST_STRING[executable->getType()] << std::endl;
        // reg_file->printRegfile();

        auto jump_type = isJumpInst(executable->getType());
        if (jump_type != NOT_JUMP)
        {
            // check branch prediction
            if (isJumpInst(executable->getType()) == CONDITIONAL_JUMP)
            {
                bool is_jumped = executable->pc_result !=
                                 executable->entry.inst_addr + 4;
                predictor->update(entry.inst_addr, is_jumped ? -1 : 1);
            }

            unsigned next_inst_addr = inst_queue.empty()
                                          ? reg_file->getPC()
                                          : inst_queue.front().inst_addr;
            if (executable->pc_result != next_inst_addr)
            {
                // catastrophic condition: branch mis-predicted
                predict_error_cnt++;
                reg_file->setPC(executable->pc_result);
                return true;
            }
        }

        delete executable;
        return false;
    }

    unsigned errorNum()
    {
        return predict_error_cnt;
    }

    void setIntIndex(unsigned num)
    {
        interrupt_index = num;
    }

    void printEntry()
    {
        std::cout << std::dec << "ROB: \n";
        for (const auto &entry : inst_queue)
        {
            std::cout << "Dest reg: " << entry.dest_reg
                      << ", Inst type: " << INST_STRING[entry.inst_type]
                      << ", is done: " << entry.is_done << '\n';
        }
        std::cout << std::endl;
    }
};

#endif