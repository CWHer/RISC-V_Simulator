#include"RISC-V.h"
#include"instruction.hpp"
#include"register.hpp"
#include"memory.hpp"


#ifndef _EXECUTE_
#define _EXECUTE_
class Execute
{
    private:
        Instruction *opt;
        Register *reg;
        unsigned addr;
        unsigned sext(unsigned x,int n) //sign-extend:将扩展后的数据的高(32-n)位置为立即数的最高位。
        {
            return (x>>n)&1?x|0xffffffff>>n<<n:x;
        }     
        unsigned setlow0(unsigned x) {return (x|1)^1;}
    public:
        Execute(Instruction *_opt,Register *_reg):reg(_reg),opt(_opt)
        {

        }
        void run()
        {
            unsigned &pc=reg->pc;
            unsigned rd=opt->rd,shamt=opt->rs2;
            unsigned rs1=opt->rs1,rs2=opt->rs2;
            unsigned imm=opt->imm;
            switch (opt->type)
            {
                case LUI:reg->data[rd]=imm;break;
                case AUIPC:reg->data[rd]=pc+imm;break;
                //control instructions begin
                //jump
                case JAL:   //J type
                {
                    reg->data[rd]=pc+4;
                    pc+=sext(imm,21);
                    break;
                }
                case JALR:  //I type
                {
                    reg->data[rd]=pc+4;
                    pc=setlow0(reg->data[rs1]+sext(imm,12));
                    break;
                }
                //branch    //B type
                case BEQ:pc+=sext(imm,13)*(reg->data[rs1]==reg->data[rs2]);break;
                case BNE:pc+=sext(imm,13)*(reg->data[rs1]!=reg->data[rs2]);break;
                case BLT:pc+=sext(imm,13)*((int)reg->data[rs1]<(int)reg->data[rs2]);break;
                case BGE:pc+=sext(imm,13)*((int)reg->data[rs1]>(int)reg->data[rs2]);break;
                case BLTU:pc+=sext(imm,13)*(reg->data[rs1]<reg->data[rs2]);break;
                case BGEU:pc+=sext(imm,13)*(reg->data[rs1]>reg->data[rs2]);break;
                //load instructions begin   //I type
                case LB: 
                case LH: 
                case LW: 
                case LBU: 
                case LHU:
                case SB:    //S type
                case SH:
                case SW:addr=reg->data[rs1]+sext(imm,12);break;
                //arithmetic and logic instructions begin
                //I type
                case ADDI:reg->data[rd]=reg->data[rs1]+sext(imm,12);break;
                case SLTI:reg->data[rd]=((int)reg->data[rs1]<(int)sext(imm,12));break;
                case SLTIU:reg->data[rd]=(reg->data[rs1]<sext(imm,12));break;
                case XORI:reg->data[rd]=reg->data[rs1]^sext(imm,12);break;
                case ORI:reg->data[rd]=reg->data[rs1]|sext(imm,12);break;
                case ANDI:reg->data[rd]=reg->data[rs1]&sext(imm,12);break;
                case SLLI:reg->data[rd]=reg->data[rs1]<<shamt;break;    //对于 RV32I，仅当 shamt[5]=0 时，指令才是有效的。???
                case SRLI:reg->data[rd]=reg->data[rs1]>>shamt;break;
                case SRAI:reg->data[rd]=(reg->data[rs1]>>shamt)|(reg->data[rs1]>>31<<31);break;
                //the original sign bit is copied into the vacated upper bits   ???
                //B type
                case ADD:reg->data[rd]=reg->data[rs1]+reg->data[rs2];break;
                case SUB:reg->data[rd]=reg->data[rs1]-reg->data[rs2];break;
                case SLL:reg->data[rd]=reg->data[rs1]<<reg->data[rs2];break;
                case SLT:reg->data[rd]=((int)reg->data[rs1]<(int)reg->data[rs2]);break;
                case SLTU:reg->data[rd]=(reg->data[rs1]<reg->data[rs2]);break;
                case XOR:reg->data[rd]=reg->data[rs1]^reg->data[rs2];break;
                case SRL:reg->data[rd]=reg->data[rs1]>>reg->data[rs2];break;
                case SRA:reg->data[rd]=(reg->data[rs1]>>reg->data[rs2])|(reg->data[rs1]>>31<<31);break;
                case OR:reg->data[rd]=reg->data[rs1]|reg->data[rs2];break;
                case AND:reg->data[rd]=reg->data[rs1]&reg->data[rs2];break;
            }
        }
        void activate()
        {

        } 
        void write_back()
        {
            
        }
};
#endif

