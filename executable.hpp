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
    friend class Execute;
    friend class MemoryAccess;
    friend class WriteBack;

private:
    Instruction inst;
    unsigned reg_result, pc_result, memory_result;
    unsigned memory_addr;

public:
    ExecWrapper() : reg_result(0),
                    pc_result(0), memory_addr(0) {}

    void init(Instruction inst)
    {
        this->inst = inst;
        reg_result = pc_result = memory_result = 0;
    }

    void reset() { inst.type = EMPTY; }

    void exec(ForwardCapsule &capsule)
    {
        auto read = [&capsule](unsigned pos, unsigned value)
        {
            return (capsule.type != EMPTY &&
                    pos > 0 && pos == capsule.dest)
                       ? capsule.reg_result
                       : value;
        };

        static const int INST_LEN = 4;
        unsigned shamt = inst.rs2, imm = inst.imm;

        switch (inst.type)
        {
        case LUI:
            reg_result = imm;
            break;
        case AUIPC:
            reg_result = inst.addr + imm;
            break;
        // control instructions begin
        // jump
        case JAL: // (J type)
        {
            reg_result = inst.addr + INST_LEN;
            pc_result = inst.addr + imm;
            break;
        }
        case JALR: // (I type)
        {
            reg_result = inst.addr + INST_LEN;
            pc_result = (read(inst.rs1, inst.Vj) + imm) & ~1; // set low bit to 0
            break;
        }
        // branch (B type)
        case BEQ:
            pc_result = (read(inst.rs1, inst.Vj) == read(inst.rs2, inst.Vk))
                            ? inst.addr + imm
                            : inst.addr + INST_LEN;
            break;
        case BNE:
            pc_result = (read(inst.rs1, inst.Vj) != read(inst.rs2, inst.Vk))
                            ? inst.addr + imm
                            : inst.addr + INST_LEN;
            break;
        case BLT:
            pc_result = ((int)read(inst.rs1, inst.Vj) < (int)read(inst.rs2, inst.Vk))
                            ? inst.addr + imm
                            : inst.addr + INST_LEN;
            break;
        case BGE:
            pc_result = ((int)read(inst.rs1, inst.Vj) >= (int)read(inst.rs2, inst.Vk))
                            ? inst.addr + imm
                            : inst.addr + INST_LEN;
            break;
        case BLTU:
            pc_result = (read(inst.rs1, inst.Vj) < read(inst.rs2, inst.Vk))
                            ? inst.addr + imm
                            : inst.addr + INST_LEN;
            break;
        case BGEU:
            pc_result = (read(inst.rs1, inst.Vj) >= read(inst.rs2, inst.Vk))
                            ? inst.addr + imm
                            : inst.addr + INST_LEN;
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
            memory_addr = read(inst.rs1, inst.Vj) + imm;
            memory_result = read(inst.rs2, inst.Vk);
            break;
        // arithmetic and logic instructions begin
        // I type
        case ADDI:
            reg_result = read(inst.rs1, inst.Vj) + imm;
            break;
        case SLTI:
            reg_result = ((int)read(inst.rs1, inst.Vj) < (int)imm);
            break;
        case SLTIU:
            reg_result = (read(inst.rs1, inst.Vj) < imm);
            break;
        case XORI:
            reg_result = read(inst.rs1, inst.Vj) ^ imm;
            break;
        case ORI:
            reg_result = read(inst.rs1, inst.Vj) | imm;
            break;
        case ANDI:
            reg_result = read(inst.rs1, inst.Vj) & imm;
            break;
        case SLLI:
            reg_result = read(inst.rs1, inst.Vj) << shamt;
            break;
        case SRLI:
            reg_result = read(inst.rs1, inst.Vj) >> shamt;
            break;
        case SRAI:
            reg_result = (read(inst.rs1, inst.Vj) >> shamt) | (read(inst.rs1, inst.Vj) >> 31 << 31);
            break;
        case ADD: // (B type)
            reg_result = read(inst.rs1, inst.Vj) + read(inst.rs2, inst.Vk);
            break;
        case SUB:
            reg_result = read(inst.rs1, inst.Vj) - read(inst.rs2, inst.Vk);
            break;
        case SLL:
            reg_result = read(inst.rs1, inst.Vj) << read(inst.rs2, inst.Vk);
            break;
        case SLT:
            reg_result = ((int)read(inst.rs1, inst.Vj) < (int)read(inst.rs2, inst.Vk));
            break;
        case SLTU:
            reg_result = (read(inst.rs1, inst.Vj) < read(inst.rs2, inst.Vk));
            break;
        case XOR:
            reg_result = read(inst.rs1, inst.Vj) ^ read(inst.rs2, inst.Vk);
            break;
        case SRL:
            reg_result = read(inst.rs1, inst.Vj) >> read(inst.rs2, inst.Vk);
            break;
        case SRA:
            reg_result = (read(inst.rs1, inst.Vj) >> read(inst.rs2, inst.Vk)) | (read(inst.rs1, inst.Vj) >> 31 << 31);
            break;
        case OR:
            reg_result = read(inst.rs1, inst.Vj) | read(inst.rs2, inst.Vk);
            break;
        case AND:
            reg_result = read(inst.rs1, inst.Vj) & read(inst.rs2, inst.Vk);
            break;
        }
    }

    void memoryAccess(Memory *mem)
    {
        switch (inst.type)
        {
        // load and store instructions begin
        // I type
        case LB:
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
        // S type
        case SB:
            mem->store(memory_addr, memory_result, 1);
            break;
        case SH:
            mem->store(memory_addr, memory_result, 2);
            break;
        case SW:
            mem->store(memory_addr, memory_result, 4);
            break;
        }
    }

    void writeBack(RegisterFile *reg_file)
    {
        unsigned dest_reg = inst.rd;
        // NOTE: HACK: pc is updated in EXE stage
        switch (inst.type)
        {
        // jump
        case JAL:
        {
            reg_file->write(dest_reg, reg_result);
            // reg_file->setPC(pc_result);
            break;
        }
        case JALR:
        {
            reg_file->write(dest_reg, reg_result);
            // reg_file->setPC(pc_result);
            break;
        }
        // branch
        case BEQ:;
        case BNE:
        case BLT:
        case BGE:
        case BLTU:
        case BGEU:
            // reg_file->setPC(pc_result);
            break;
        // load
        case LB:
        case LH:
        case LW:
        case LBU:
        case LHU:
        // arithmetic and logic instructions
        // U type
        case LUI:
        case AUIPC:
        // I type
        case ADDI:
        case SLTI:
        case SLTIU:
        case XORI:
        case ORI:
        case ANDI:
        case SLLI:
        case SRLI:
        case SRAI:
        // B type
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

    InstructionTypes getType() { return inst.type; }

    ForwardCapsule makeForward()
    {
        return ForwardCapsule(getType(), inst.rd, reg_result);
    }

    unsigned instAddr() { return inst.addr; }
};
#endif
