#ifndef _RISC_V_
#define _RISC_V_
enum Basictypes{R,I,S,B,U,J};
enum Instructiontypes
{
    LUI,AUIPC,JAL,JALR,
    BEQ,BNE,BLT,BGE,BLTU,BGEU,
    LB,LH,LW,LBU,LHU,SB,SH,SW,
    ADDI,SLTI,SLTIU,XORI,ORI,ANDI,
    SLLI,SRLI,SRAI,
    ADD,SUB,SLL,SLT,SLTU,XOR,SRL,SRA,OR,AND
};

#endif