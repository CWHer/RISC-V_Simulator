#include"RISC-V.h"
#include"instruction.hpp"
#include"register.hpp"

class Execute
{
    private:
        Instruction *opt;
        Register *reg;   
        unsigned sext(unsigned x,int n) //sign-extend   符号扩展：将扩展后的数据的高(32-n)位置为立即数的最高位。
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
            unsigned rd=opt->rd;
            unsigned rs1=opt->rs1,rs2=opt->rs2;
            unsigned imm=opt->imm;
            switch (opt->type)
            {
                case LUI:reg->data[rd]=imm;break;
                case AUIPC:reg->data[rd]=pc+imm;break;
                case JAL:
                {
                    reg->data[rd]=pc+4;
                    pc+=sext(imm,21);
                    break;
                }
                case JALR:
                {
                    reg->data[rd]=pc+4;
                    pc=setlow0(reg->data[rs1]+sext(imm,12));
                    break;
                }
                case BEQ:

            }

        }
};

