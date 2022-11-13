#ifndef __RESERVATION_STATION_HPP__
#define __RESERVATION_STATION_HPP__

#include "RISC-V.h"
#include "register_file.hpp"
#include "instruction.hpp"
#include "alu.hpp"
#include "slu.hpp"

class ReservationStation
{
    friend class ExecWrapper;

private:
    RegisterFile *reg_file;
    static const unsigned MAX_SL_RES = 2;
    static const unsigned MAX_AL_RES = 2;
    std::deque<ResEntry> SLU_res, ALU_res;

public:
    ReservationStation(RegisterFile *reg_file) : reg_file(reg_file) {}

    bool isFull(InstructionTypes type)
    {
        return isMemoryInst(type) != NOT_MEM
                   ? SLU_res.size() == MAX_SL_RES
                   : ALU_res.size() == MAX_AL_RES;
    }

    void reset()
    {
        SLU_res.clear();
        ALU_res.clear();
    }

    void push(Instruction inst, ROBEntry *dest)
    {
        ResEntry entry;
        entry.dest = dest;
        entry.inst_addr = inst.addr;
        entry.inst_type = inst.type;

        // NOTE: NOT all inst has 2 rs
        if (inst.basic_type != U && inst.basic_type != J)
        {
            std::pair<unsigned, ROBEntry *> operand;
            operand = reg_file->readReg(inst.rs1);
            entry.Vj = operand.first, entry.Qj = operand.second;
            if (inst.basic_type != I)
            {
                operand = reg_file->readReg(inst.rs2);
                entry.Vk = operand.first, entry.Qk = operand.second;
            }
            else
                entry.Vk = inst.rs2; // shamt in I-type

            // NOTE: read result from ROB
            // HACK: only consider reg_result here
            // clang-format off
            auto getExec = [](ROBEntry *entry) {
                return reinterpret_cast<ExecWrapper *>(entry->executable);
            };
            // clang-format on
            if (entry.Qj != nullptr && entry.Qj->is_done)
            {
                entry.Vj = getExec(entry.Qj)->reg_result;
                entry.Qj = nullptr;
            }
            if (entry.Qk != nullptr && entry.Qk->is_done)
            {
                entry.Vk = getExec(entry.Qk)->reg_result;
                entry.Qk = nullptr;
            }
        }
        entry.imm = inst.imm;
        isMemoryInst(inst.type) != NOT_MEM
            ? SLU_res.push_back(entry)
            : ALU_res.push_back(entry);
    }

    void remove(ResEntry entry)
    {
        auto res_station = isMemoryInst(entry.inst_type) != NOT_MEM ? &SLU_res : &ALU_res;
        res_station->erase(std::find(res_station->begin(), res_station->end(), entry));
    }

    void broadcastOperand(ROBEntry *rob_entry, unsigned val)
    {
        for (auto &entry : SLU_res)
        {
            if (entry.Qj == rob_entry)
                entry.Qj = nullptr, entry.Vj = val;
            if (entry.Qk == rob_entry)
                entry.Qk = nullptr, entry.Vk = val;
        }

        for (auto &entry : ALU_res)
        {
            if (entry.Qj == rob_entry)
                entry.Qj = nullptr, entry.Vj = val;
            if (entry.Qk == rob_entry)
                entry.Qk = nullptr, entry.Vk = val;
        }
    }

    void sendReadyEntry(ALUnit *alu, SLUnit *slu)
    {
        static const int SLU_CYCLE = 3, ALU_CYCLE = 1;

        if (!slu->isBusy())
            for (auto &entry : SLU_res)
                if (entry.Qj == nullptr && entry.Qk == nullptr)
                {
                    slu->send(&entry);
                    slu->start(SLU_CYCLE);
                    this->remove(entry);
                    break;
                }

        if (!alu->isBusy())
            for (auto &entry : ALU_res)
                if (entry.Qj == nullptr && entry.Qk == nullptr)
                    if (!alu->isBusy())
                    {
                        alu->send(&entry);
                        alu->start(ALU_CYCLE);
                        this->remove(entry);
                        break;
                    }
    }

    void printEntry()
    {
        std::cout << "SLU Reservation: " << std::endl;
        for (auto &entry : SLU_res)
            std::cout << "Inst addr: " << entry.inst_addr
                      << ", Inst type: " << INST_STRING[entry.inst_type]
                      << ", Vj: " << entry.Vj << ", Qj: " << entry.Qj
                      << ", Vk: " << entry.Vk << ", Qk: " << entry.Qk << '\n';
        std::cout << std::endl;

        std::cout << "ALU Reservation: " << std::endl;
        for (auto &entry : ALU_res)
            std::cout << "Inst addr: " << entry.inst_addr
                      << ", Inst type: " << INST_STRING[entry.inst_type]
                      << ", Vj: " << entry.Vj << ", Qj: " << entry.Qj
                      << ", Vk: " << entry.Vk << ", Qk: " << entry.Qk << '\n';
        std::cout << std::endl;
    }
};

#endif
