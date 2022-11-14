#ifndef __EXECUTABLE_HPP__
#define __EXECUTABLE_HPP__

#include "RISC-V.h"
#include "instruction.hpp"
#include "register_file.hpp"
#include "memory.hpp"

// For the brevity of the code,
//  I move Arithmetic Calculation, Memory Access,
//  and Write Back into this abstract class.
class ExecWrapper
{
    friend class ReorderBuffer;
    friend class ReservationStation;
    friend class CommonDataBus;

private:
    ResEntry entry;
    unsigned reg_result, pc_result;
    unsigned memory_addr;

public:
    ExecWrapper() : entry(), reg_result(0),
                    pc_result(0), memory_addr(0) {}

    void init(ResEntry *entry)
    {
        this->entry = *entry;
        entry->dest->executable = this;
        reg_result = pc_result = 0;
    }

    void exec()
    {
        static const int INST_LEN = 4;
        unsigned shamt = entry.Vk, imm = entry.imm;

        switch (entry.inst_type)
        {
        case LUI:
            reg_result = imm;
            break;
        case AUIPC:
            reg_result = entry.inst_addr + imm;
            break;
        // control instructions begin
        // jump
        case JAL: // (J type)
        {
            reg_result = entry.inst_addr + INST_LEN;
            pc_result = entry.inst_addr + imm;
            break;
        }
        case JALR: // (I type)
        {
            reg_result = entry.inst_addr + INST_LEN;
            pc_result = ((entry.Vj + imm) | 1) ^ 1; // set low bit to 0
            break;
        }
        // branch (B type)
        // if not jump pc will also goto next
        case BEQ:
            pc_result = entry.Vj == entry.Vk
                            ? entry.inst_addr + imm
                            : entry.inst_addr + INST_LEN;
            break;
        case BNE:
            pc_result = entry.Vj != entry.Vk
                            ? entry.inst_addr + imm
                            : entry.inst_addr + INST_LEN;
            break;
        case BLT:
            pc_result = (int)entry.Vj < (int)entry.Vk
                            ? entry.inst_addr + imm
                            : entry.inst_addr + INST_LEN;
            break;
        case BGE:
            pc_result = (int)entry.Vj >= (int)entry.Vk
                            ? entry.inst_addr + imm
                            : entry.inst_addr + INST_LEN;
            break;
        case BLTU:
            pc_result = entry.Vj < entry.Vk
                            ? entry.inst_addr + imm
                            : entry.inst_addr + INST_LEN;
            break;
        case BGEU:
            pc_result = entry.Vj >= entry.Vk
                            ? entry.inst_addr + imm
                            : entry.inst_addr + INST_LEN;
            break;
        // load & store instructions begin
        case LB: // (I type)
        case LH:
        case LW:
        case LBU:
        case LHU:
        case SB: // (S type)
        case SH:
        case SW:
            memory_addr = entry.Vj + imm;
            break;
        // arithmetic and logic instructions begin
        case ADDI: // (I type)
            reg_result = entry.Vj + imm;
            break;
        case SLTI:
            reg_result = ((int)entry.Vj < (int)imm);
            break;
        case SLTIU:
            reg_result = (entry.Vj < imm);
            break;
        case XORI:
            reg_result = entry.Vj ^ imm;
            break;
        case ORI:
            reg_result = entry.Vj | imm;
            break;
        case ANDI:
            reg_result = entry.Vj & imm;
            break;
        case SLLI:
            reg_result = entry.Vj << shamt;
            break;
        case SRLI:
            reg_result = entry.Vj >> shamt;
            break;
        case SRAI:
            reg_result = (entry.Vj >> shamt) | (entry.Vj >> 31 << 31);
            break;
        case ADD: // (B type)
            reg_result = entry.Vj + entry.Vk;
            break;
        case SUB:
            reg_result = entry.Vj - entry.Vk;
            break;
        case SLL:
            reg_result = entry.Vj << entry.Vk;
            break;
        case SLT:
            reg_result = ((int)entry.Vj < (int)entry.Vk);
            break;
        case SLTU:
            reg_result = (entry.Vj < entry.Vk);
            break;
        case XOR:
            reg_result = entry.Vj ^ entry.Vk;
            break;
        case SRL:
            reg_result = entry.Vj >> entry.Vk;
            break;
        case SRA:
            reg_result = (entry.Vj >> entry.Vk) | (entry.Vj >> 31 << 31);
            break;
        case OR:
            reg_result = entry.Vj | entry.Vk;
            break;
        case AND:
            reg_result = entry.Vj & entry.Vk;
            break;
        }
    }

    void memoryAccess(Memory *mem)
    {
        switch (entry.inst_type)
        {
        // load and store instructions begin
        case LB: // (I type)
            reg_result = sext(mem->load(memory_addr, 1), 7);
            break;
        case LH:
            reg_result = sext(mem->load(memory_addr, 2), 15);
            break;
        case LW:
            reg_result = mem->load(memory_addr, 4);
            break;
        case LBU:
            reg_result = mem->load(memory_addr, 1);
            break;
        case LHU:
            reg_result = mem->load(memory_addr, 2);
            break;
        }
    }

    void writeBack(unsigned dest_reg,
                   Memory *memory, RegisterFile *reg_file)
    {
        switch (entry.inst_type)
        {
        // jump
        // NOTE: HACK: pc is updated in Issue / ROB stages
        case JAL: // J type
        {
            reg_file->write(dest_reg, reg_result);
            // reg_file->setPC(pc_result);
            break;
        }
        case JALR: // I type
        {
            // HACK: JALR causes an entire stall
            reg_file->write(dest_reg, reg_result);
            reg_file->setPC(pc_result);
            break;
        }

        // store
        case SB:
            memory->store(memory_addr, entry.Vk, 1);
            break;
        case SH:
            memory->store(memory_addr, entry.Vk, 2);
            break;
        case SW:
            memory->store(memory_addr, entry.Vk, 4);
            break;
        // load
        case LB:
        case LH:
        case LW:
        case LBU:
        case LHU:
        // arithmetic and logic instructions begin
        case LUI:
        case AUIPC:
        case ADDI:
        case SLTI:
        case SLTIU:
        case XORI:
        case ORI:
        case ANDI:
        case SLLI:
        case SRLI:
        case SRAI:
        case ADD:
        case SUB:
        case SLL:
        case SLT:
        case SLTU:
        case XOR:
        case SRL:
        case SRA:
        case OR:
        case AND:
            reg_file->write(dest_reg, reg_result);
            break;
        }
    }

    InstructionTypes getType() const
    {
        return entry.inst_type;
    }
};
#endif
