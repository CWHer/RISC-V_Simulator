#ifndef __RISC_V__
#define __RISC_V__

#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>
#include <iomanip>
#include <map>
#include <set>
#include <cassert>

// clang-format off
enum BasicTypes
{
    R, I, S, B, U, J
};

enum InstructionTypes
{
    LUI, AUIPC, JAL, JALR,
    BEQ, BNE, BLT, BGE, BLTU, BGEU,
    LB, LH, LW, LBU, LHU, SB, SH, SW,
    ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI, 
    ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND,
    EMPTY
};
// clang-format on

struct ForwardCapsule
{
    InstructionTypes type;
    unsigned dest, reg_result;

    ForwardCapsule() : type(EMPTY), dest(0), reg_result(0) {}

    ForwardCapsule(InstructionTypes type,
                   unsigned rd, unsigned reg_result)
        : type(type), dest(rd), reg_result(reg_result) {}

    void reset()
    {
        type = EMPTY;
        dest = reg_result = 0;
    }
};

const char *INST_STRING[] = {
    "LUI", "AUIPC", "JAL", "JALR",
    "BEQ", "BNE", "BLT", "BGE", "BLTU", "BGEU",
    "LB", "LH", "LW", "LBU", "LHU", "SB", "SH", "SW",
    "ADDI", "SLTI", "SLTIU", "XORI", "ORI", "ANDI", "SLLI", "SRLI", "SRAI",
    "ADD", "SUB", "SLL", "SLT", "SLTU", "XOR", "SRL", "SRA", "OR", "AND",
    "EMPTY"};

// utils
unsigned sext(unsigned x, int n) // sign-extend
{
    return (x >> n) & 1 ? x | 0xffffffff >> n << n : x;
}

enum JumpTypes
{
    NOT_JUMP,
    CONDITIONAL_JUMP,
    UNCONDITIONAL_JUMP
};

JumpTypes isJumpInst(InstructionTypes type)
{
    switch (type)
    {
    case BEQ:
    case BNE:
    case BLT:
    case BGE:
    case BLTU:
    case BGEU:
        return CONDITIONAL_JUMP;
    case JAL:
    case JALR:
        return UNCONDITIONAL_JUMP;
    default:
        return NOT_JUMP;
    }
}

enum MemTypes
{
    NOT_MEM,
    MEM_LOAD,
    MEM_STORE
};

MemTypes isMemoryInst(InstructionTypes type)
{
    switch (type)
    {
    case LB:
    case LH:
    case LW:
    case LBU:
    case LHU:
        return MEM_LOAD;
    case SB:
    case SH:
    case SW:
        return MEM_STORE;
    default:
        return NOT_MEM;
    }
}

bool isRegDest(InstructionTypes type)
{
    std::set<InstructionTypes> reg_dest_types = {
        LUI, AUIPC, JAL, JALR,
        LB, LH, LW, LBU, LHU,
        ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI,
        ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND};

    return reg_dest_types.count(type) > 0;
}

#endif