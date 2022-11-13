#ifndef __RISC_V__
#define __RISC_V__

#include <utility>
#include <queue>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <iomanip>
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

// entry in ROB
struct ROBEntry
{
    unsigned dest_reg;
    unsigned inst_addr;
    InstructionTypes inst_type;
    bool is_done;
    // pointer to the corresponding Executable
    void *executable;
    ROBEntry() : dest_reg(0), inst_type(EMPTY),
                 is_done(false), executable(nullptr) {}
};

// entry in reservation station
struct ResEntry // node in res station
{
    ROBEntry *dest;
    unsigned inst_addr;
    InstructionTypes inst_type;
    unsigned Vj, Vk;
    ROBEntry *Qj, *Qk;
    unsigned imm;

    ResEntry() : dest(nullptr), inst_addr(0), inst_type(EMPTY),
                 Vj(0), Vk(0), Qj(nullptr), Qk(nullptr), imm(0) {}

    bool operator==(const ResEntry &rhs) const
    {
        return inst_addr == rhs.inst_addr && inst_type == rhs.inst_type;
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

#endif