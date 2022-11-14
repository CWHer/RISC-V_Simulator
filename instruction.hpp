#ifndef __INSTRUCTION_HPP__
#define __INSTRUCTION_HPP__

#include "RISC-V.h"
#include "memory.hpp"
#include "register_file.hpp"
#include "predictor.hpp"

class Instruction
{
    friend class ExecWrapper;
    friend class InstructionFetch;
    friend class InstructionDecode;

private:
    unsigned addr;
    unsigned inst_bytes;
    unsigned rs1, rs2, rd;
    unsigned Vj, Vk;
    unsigned imm;
    BasicTypes basic_type;
    InstructionTypes type;

public:
    Instruction() : addr(0), inst_bytes(0),
                    rs1(0), rs2(0), rd(0), imm(0),
                    basic_type(R), type(EMPTY) {}

    bool fetch(Memory *memory, RegisterFile *reg_files)
    {
        static const unsigned STOP_INST = 0x0ff00513;
        static const unsigned INST_LEN = 4; // 4 bytes

        addr = reg_files->getPC();
        inst_bytes = memory->load(addr, INST_LEN);

        return inst_bytes == STOP_INST;
    }

    // HACK: is this instruction a branch?
    bool fastDecode()
    {
        bool is_branch = false;
        unsigned op_code = inst_bytes & 0x7f;
        switch (op_code)
        {
        case 0b1101111: // JAL
            basic_type = J;
            is_branch = true;
            break;
        case 0b1100111: // JALR (FIXME: not supported, always wrong)
            basic_type = I;
            is_branch = true;
            break;
        case 0b1100011: // BEQ, BNE, BLT, BGE, BLTU, BGEU
            basic_type = B;
            is_branch = true;
            break;
        }

        // calculate immediate
        switch (basic_type)
        {
        case B:
            imm = sext((inst_bytes >> 8 << 1 & 0x1f) +
                           (inst_bytes >> 25 << 5 & 0x7ff) +
                           ((inst_bytes >> 7 & 1) << 11) +
                           (inst_bytes >> 31 << 12),
                       12);
            break;
        case J:
            imm = sext((inst_bytes >> 21 << 1 & 0x7ff) +
                           ((inst_bytes >> 20 & 0x1) << 11) +
                           ((inst_bytes >> 12 & 0xff) << 12) +
                           (inst_bytes >> 31 << 20),
                       20);
            break;
        }
        return is_branch;
    }

    void decode()
    {
        // unsigned op_code = inst_bytes & 127;
        unsigned op_code = inst_bytes & 0x7f;
        unsigned funct3 = inst_bytes >> 12 & 0x7;
        unsigned funct7 = inst_bytes >> 25 & 0x7f;
        switch (op_code)
        {
        case 0b0110111:
            basic_type = U, type = LUI;
            break;
        case 0b0010111:
            basic_type = U, type = AUIPC;
            break;
        case 0b1101111:
            basic_type = J, type = JAL;
            break;
        case 0b1100111:
            basic_type = I, type = JALR;
            break;
        case 0b1100011:
        {
            // clang-format off
            static const std::unordered_map<int, InstructionTypes> type_map = {
                {0b000, BEQ}, {0b001, BNE}, {0b100, BLT}, {0b101, BGE}, {0b110, BLTU}, {0b111, BGEU}
            };
            // clang-format on
            type = type_map.at(funct3);
            basic_type = B;
            break;
        }
        case 0b0000011:
        {
            // clang-format off
            static const std::unordered_map<int, InstructionTypes> type_map = {
                {0b000, LB}, {0b001, LH}, {0b010, LW}, {0b100, LBU}, {0b101, LHU}
            };
            // clang-format on
            type = type_map.at(funct3);
            basic_type = I;
            break;
        }
        case 0b0100011:
        {
            // clang-format off
            static const std::unordered_map<int, InstructionTypes> type_map = {
                {0b000, SB}, {0b001, SH}, {0b010, SW}
            };
            // clang-format on
            type = type_map.at(funct3);
            basic_type = S;
            break;
        }
        case 0b0010011:
        {
            // clang-format off
            static const std::unordered_map<int, InstructionTypes> type_map = {
                {0b000, ADDI}, {0b010, SLTI}, {0b011, SLTIU}, {0b100, XORI},
                {0b110, ORI}, {0b111, ANDI}, {0b001, SLLI}
            };
            // clang-format on
            switch (funct3)
            {
            case 0b101:
                type = funct7 == 0 ? SRLI : SRAI;
                break;
            default:
                type = type_map.at(funct3);
                break;
            }
            basic_type = I;
            break;
        }

        case 0b0110011:
        {
            // clang-format off
            static const std::unordered_map<int, InstructionTypes> type_map = {
                {0b001, SLL}, {0b010, SLT}, {0b011, SLTU}, {0b100, XOR}, {0b110, OR}, {0b111, AND}
            };
            // clang-format on

            switch (funct3)
            {
            case 0b000:
                type = funct7 == 0 ? ADD : SUB;
                break;
            case 0b101:
                type = funct7 == 0 ? SRL : SRA;
                break;
            default:
                type = type_map.at(funct3);
                break;
            }
            basic_type = R;
            break;
        }
        }

        rs1 = inst_bytes >> 15 & 0x1f;
        rs2 = inst_bytes >> 20 & 0x1f;
        rd = inst_bytes >> 7 & 0x1f;

        // calculate immediate
        switch (basic_type)
        {
        case I:
            imm = sext(inst_bytes >> 20, 11);
            break;
        case S:
            imm = sext((inst_bytes >> 7 & 0x1f) + (inst_bytes >> 25 << 5), 11);
            break;
        case B:
            imm = sext((inst_bytes >> 8 << 1 & 0x1f) +
                           (inst_bytes >> 25 << 5 & 0x7ff) +
                           ((inst_bytes >> 7 & 1) << 11) +
                           (inst_bytes >> 31 << 12),
                       12);
            break;
        case U:
            imm = inst_bytes >> 12 << 12;
            break;
        case J:
            imm = sext((inst_bytes >> 21 << 1 & 0x7ff) +
                           ((inst_bytes >> 20 & 0x1) << 11) +
                           ((inst_bytes >> 12 & 0xff) << 12) +
                           (inst_bytes >> 31 << 20),
                       20);
            break;
        }
    }

    void readReg(RegisterFile *reg_file)
    {
        switch (type)
        {
        case JALR:
        // immediate instructions
        case ADDI:
        case SLTI:
        case SLTIU:
        case XORI:
        case ORI:
        case ANDI:
        case SLLI:
        case SRLI:
        case SRAI:
            Vj = reg_file->read(rs1);
            break;
        // branch (B type)
        case BEQ:
        case BNE:
        case BLT:
        case BGE:
        case BLTU:
        case BGEU:
        // load & store instructions
        case LB:
        case LH:
        case LW:
        case LBU:
        case LHU:
        case SB:
        case SH:
        case SW:
        // arithmetic and logic instructions
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
            Vj = reg_file->read(rs1);
            Vk = reg_file->read(rs2);
            break;
        }
    };
    
    InstructionTypes getType() const { return type; }
};

#endif