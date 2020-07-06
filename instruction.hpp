#include<cstdio>
#include"RISC-V.h"


#ifndef _INSTRUCTION_
#define _INSTRUCTION_
class Instruction
{
    friend class RISC_V;
    friend class Execute;
    private:
        unsigned seq;
        unsigned opcode,func3,func7;
        unsigned rs1,rs2,rd;
        unsigned imm;
        Basictypes basictype;
        Instructiontypes type;
        char get()
        {
            char ch=getchar();
            while (!(ch>='0'&&ch<='9')&&!(ch>='A'&&ch<='F')) ch=getchar();
            return ch;
        }
        unsigned char2int(char ch)
        {
            return ch>='A'?ch-'A'+10:ch-'0';
        }
    public:
        Instruction(unsigned _seq=0):seq(_seq)
        {
            _seq=imm=0;
            rs1=rs2=rd=0;
            opcode=func3=func7=0;
        }
        void read()
        {
            for(int i=0;i<4;++i)
            {
                seq+=char2int(get())*(1<<(i*8+4));
                seq+=char2int(get())*(1<<(i*8));
            }
                
        }
        void decode()
        {
            opcode=seq&127;
            func3=(seq>>12)&7;
            func7=(seq>>25)&127;
            switch (opcode)
            {
                case 55:basictype=U,type=LUI;break;
                case 23:basictype=U,type=AUIPC;break;
                case 111:basictype=J,type=JAL;break;
                case 103:basictype=I,type=JALR;break;
                case 99: 	
                {
                    switch (func3) 
                    {
						case 0:type=BEQ;break;		
						case 1:type=BNE;break; 		
						case 4:type=BLT;break;		
						case 5:type=BGE;break;		
						case 6:type=BLTU;break;		
						case 7:type=BGEU;break;		
					}
                    basictype=B;
                    break;
                }												
				case 3:	
                {
                    switch (func3) 
                    {
						case 0:type=LB;break;			
						case 1:type=LH;break;		
						case 2:type=LW;break;		
						case 4:type=LBU;break;	
						case 5:type=LHU;break;		
					}
                    basictype=I;
                    break;
                }												
				case 35:	
                {							
					switch (func3) 
                    {
						case 0:type=SB;break;			
						case 1:type=SH;break;			
						case 2:type=SW;break;			
					}
                    basictype=S;
					break;
                }
				case 19:
                {
					switch (func3) 
                    {
						case 0:type=ADDI;break;	
						case 1:type=SLLI;break;	
						case 2:type=SLTI;break;		
						case 3:type=SLTIU;break;	
						case 4:type=XORI;break;
						case 5:type=func7?SRAI:SRLI;break;
						case 6:type=ORI;break;		
						case 7:type=ANDI;break;	
					}
                    basictype=I;
					break;
                }
				case 51:
                {
                	switch(func3) 
                    {
						case 0:type=func7?SUB:ADD;break;
						case 1:type=SLL;break;		
						case 2:type=SLT;break;		
						case 3:type=SLTU;break;		
						case 4:type=XOR;break;		
						case 5:type=func7?SRA:SRL;									
						case 6:type=OR;break;			
						case 7:type=AND;break;		
					}
                    basictype=R;
					break;
                }
            }
            rs1=(seq>>15)&31;
            rs2=(seq>>20)&31;
            rd=(seq>>7)&31;
            switch (basictype) 
            {
                case I:imm=seq>>20;break;
                case S:imm=((seq>>7)&31)+(seq>>25<<5);break;
                case B:imm=((seq>>8<<1)&15)+((seq>>25<<5)&1023)+((seq>>7)&1<<11)+(seq>>31<<12);break;
                case U:imm=seq>>12<<12;break;
                case J:imm=((seq>>21<<1)&1023)+((seq>>20)&1<<11)+((seq>>12)&255<<12)+(seq>>31<<20);break;
            }
        }
};
#endif