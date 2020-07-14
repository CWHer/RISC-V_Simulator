#ifndef __RESERVATIONSTATION__
#define __RESERVATIONSTATION__

#include"RISC-V.h"
#include"register.hpp"
#include"instruction.hpp"

class ReservationStation
{
    friend class Executor;
    private:
        Register *reg;
        static const unsigned SLN=2;
        static const unsigned ALN=2;
        std::deque<Resnode> SLres,ALres;
    public:
        ReservationStation(Register *_reg):reg(_reg) {}
        bool full(Instructiontypes type)
        {   
            return isSL(type)?SLres.size()==SLN:ALres.size()==ALN;
        }
        void push(Instruction opt)
        {
            Resnode t;
            t.num=opt.num,t.pc=opt.pc;
            t.Op=opt.type,t.rd=opt.rd;

            //notice:not all inst has 2 rs

            std::pair<unsigned,Resnode*> data;
            data=reg->getdata(opt.rs1);
            t.Vj=data.first,t.Qj=data.second;
            data=reg->getdata(opt.rs2);
            t.Vk=data.first,t.Qk=data.second;
            t.A=opt.imm,t.isBusy=1;
            if (isSL(opt.type))
                SLres.push_back(t);
            else
                ALres.push_back(t);
        }
        void update(Resnode *opt,unsigned val)   //update by opt
        {
            std::deque<Resnode>::iterator it;
            for(it=SLres.begin();it!=SLres.end();it++)
            {
                if (it->Qj==opt)
                    it->Qj=NULL,it->Vj=val;
                if (it->Qk==opt)
                    it->Qj=NULL,it->Vj=val;
            }
            for(it=ALres.begin();it!=ALres.end();it++)
            {
                if (it->Qj==opt)
                    it->Qj=NULL,it->Vj=val;
                if (it->Qk==opt)
                    it->Qj=NULL,it->Vj=val;
            }
        }
};


#endif
