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
        Memory *mem;
        unsigned addr;
        unsigned temp_result;
        unsigned sext(unsigned x,int n) //sign-extend: fill imm with (32-n)位置为立即数的最高位。
        {
            return (x>>n)&1?x|0xffffffff>>n<<n:x;
        }     
        unsigned setlow0(unsigned x) {return (x|1)^1;}
    public:
        Execute(Instruction *_opt,Register *_reg,Memory *_mem):reg(_reg),opt(_opt),mem(_mem) {}
        void run()
        {
            unsigned &pc=reg->getpc();
            unsigned rd=opt->rd,shamt=opt->rs2;
            unsigned rs1=opt->rs1,rs2=opt->rs2;
            unsigned imm=opt->imm;
            switch (opt->type)
            {
                case LUI:temp_result=imm;break;
                case AUIPC:temp_result=pc+imm;break;
                //control instructions begin
                //jump
                case JAL:   //J type
                {
                    temp_result=pc+4;
                    pc+=sext(imm,21);
                    break;
                }
                case JALR:  //I type
                {
                    temp_result=pc+4;
                    pc=setlow0(reg->getdata(rs1)+sext(imm,12));
                    break;
                }
                //branch    //B type
                case BEQ:pc+=sext(imm,13)*(reg->getdata(rs1)==reg->getdata(rs2));break;
                case BNE:pc+=sext(imm,13)*(reg->getdata(rs1)!=reg->getdata(rs2));break;
                case BLT:pc+=sext(imm,13)*((int)reg->getdata(rs1)<(int)reg->getdata(rs2));break;
                case BGE:pc+=sext(imm,13)*((int)reg->getdata(rs1)>(int)reg->getdata(rs2));break;
                case BLTU:pc+=sext(imm,13)*(reg->getdata(rs1)<reg->getdata(rs2));break;
                case BGEU:pc+=sext(imm,13)*(reg->getdata(rs1)>reg->getdata(rs2));break;
                //load instructions begin   //I type
                case LB: 
                case LH: 
                case LW: 
                case LBU: 
                case LHU:
                case SB:    //S type
                case SH:
                case SW:addr=reg->getdata(rs1)+sext(imm,12);break;
                //arithmetic and logic instructions begin
                //I type
                case ADDI:temp_result=reg->getdata(rs1)+sext(imm,12);break;
                case SLTI:temp_result=((int)reg->getdata(rs1)<(int)sext(imm,12));break;
                case SLTIU:temp_result=(reg->getdata(rs1)<sext(imm,12));break;
                case XORI:temp_result=reg->getdata(rs1)^sext(imm,12);break;
                case ORI:temp_result=reg->getdata(rs1)|sext(imm,12);break;
                case ANDI:temp_result=reg->getdata(rs1)&sext(imm,12);break;
                case SLLI:temp_result=reg->getdata(rs1)<<shamt;break;    
                case SRLI:temp_result=reg->getdata(rs1)>>shamt;break;
                case SRAI:temp_result=(reg->getdata(rs1)>>shamt)|(reg->getdata(rs1)>>31<<31);break;
                //the original sign bit is copied into the vacated upper bits   ???
                //B type
                case ADD:temp_result=reg->getdata(rs1)+reg->getdata(rs2);break;
                case SUB:temp_result=reg->getdata(rs1)-reg->getdata(rs2);break;
                case SLL:temp_result=reg->getdata(rs1)<<reg->getdata(rs2);break;
                case SLT:temp_result=((int)reg->getdata(rs1)<(int)reg->getdata(rs2));break;
                case SLTU:temp_result=(reg->getdata(rs1)<reg->getdata(rs2));break;
                case XOR:temp_result=reg->getdata(rs1)^reg->getdata(rs2);break;
                case SRL:temp_result=reg->getdata(rs1)>>reg->getdata(rs2);break;
                case SRA:temp_result=(reg->getdata(rs1)>>reg->getdata(rs2))|(reg->getdata(rs1)>>31<<31);break;
                case OR:temp_result=reg->getdata(rs1)|reg->getdata(rs2);break;
                case AND:temp_result=reg->getdata(rs1)&reg->getdata(rs2);break;
            }
        }
        void memory_access()
        {
            switch (opt->type)
            {
                //load and store instructions begin   
                //I type
                case LB:temp_result=sext(mem->load(addr,1),8);break;
                case LH:temp_result=sext(mem->load(addr,2),16);break;
                case LW:temp_result=mem->load(addr,4);break;
                case LBU:temp_result=mem->load(addr,1);break;
                case LHU:temp_result=mem->load(addr,2);break;
                //S type
                case SB:mem->store(addr,reg->getdata(opt->rs2),1);break;
                case SH:mem->store(addr,reg->getdata(opt->rs2),2);break;
                case SW:mem->store(addr,reg->getdata(opt->rs2),4);break;
            }
        } 
        void write_back()
        {
            unsigned rd=opt->rd;
            switch (opt->type)
            {
                case LUI:
                case AUIPC:
                // reg->getdata(rd]=temp_result;break;
                //control instructions begin
                //jump
                case JAL:   //J type
                // {
                    // reg->getdata(rd]=pc+4;
                    // pc+=sext(imm,21);
                    // break;
                // }
                case JALR:  //I type
                // {
                //     reg->getdata(rd]=pc+4;
                //     pc=setlow0(reg->getdata(rs1]+sext(imm,12));
                //     break;
                // }
                //load and store instructions begin   
                //I type
                case LB: 
                case LH: 
                case LW: 
                case LBU: 
                case LHU:
                //arithmetic and logic instructions begin
                //I type
                case ADDI:
                case SLTI:
                case SLTIU:
                case XORI:
                case ORI:
                case ANDI:
                case SLLI:
                case SRLI:
                case SRAI:
                //the original sign bit is copied into the vacated upper bits   ???
                //B type
                case ADD:
                case SUB:
                case SLL:
                case SLT:
                case SLTU:
                case XOR:
                case SRL:
                case SRA:
                case OR:
                case AND:reg->setdata(rd,temp_result);break;
            }
        }
};
#endif

