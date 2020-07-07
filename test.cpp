#include"instruction.hpp"
#include"execute.hpp"
#include"register.hpp"
#include"memory.hpp"
#include"RISC-V.h"
#include<iostream>
Register reg;
Instruction opt;
Memory mem;
const char *str[]=
    {
        "LUI","AUIPC","JAL","JALR",
        "BEQ","BNE","BLT","BGE","BLTU","BGEU",
        "LB","LH","LW","LBU","LHU","SB","SH","SW",
        "ADDI","SLTI","SLTIU","XORI","ORI","ANDI",
        "SLLI","SRLI","SRAI",
        "ADD","SUB","SLL","SLT","SLTU","XOR","SRL","SRA","OR","AND"
    };
unsigned sext(unsigned x,int n) //sign-extend: fill imm with (32-n)位置为立即数的最高位。
{
    return (x>>n)&1?x|0xffffffff>>n<<n:x;
}   
int main()
{
    // unsigned seq=0xf69ff0ef;
    // std::cout<<(seq>>21<<1&1023)<<std::endl;
    // std::cout<<((seq>>20&1)<<11)<<std::endl;
    // std::cout<<((seq>>12&255)<<12)<<std::endl;
    // std::cout<<(seq>>31<<20)<<std::endl;
    // int imm=(seq>>21<<1&2047)+((seq>>20&1)<<11)+((seq>>12&255)<<12)+(seq>>31<<20);
    // std::cout<<sext(2095976,20)<<std::endl;
    int cnt=0;
    // freopen("out","w",stdout);
    mem.init_read();
    // freopen("in","r",stdin);
    while (!opt.empty())
    {
        opt.fetch(&mem,&reg);
        opt.decode();
        cnt++;
        // std::cout<<++cnt<<' '<<str[opt.gettype()]<<std::endl;
        // if (cnt==31855)
        // {
            // puts("1");
        // }
        Execute exe(&opt,&reg,&mem);
        exe.run();
        exe.memory_access();
        exe.write_back();
        // t.read();
        // t.decode();
        // Execute e(&t,&reg);
        // e.run(); 
    }
    std::cout<<cnt<<std::endl;
    std::cout<<reg.output()<<std::endl;
    return 0;
}