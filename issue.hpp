#ifndef __ISSUE__
#define __ISSUE__

#include"RISC-V.h"
#include"instruction.hpp"
#include"register.hpp"
#include"reservationstation.hpp"

class Issue
{
    private:
        Register *reg;
    public:
        Issue(Register *_reg):reg(_reg) {}
        void run(ReservationStation *res)  //if res station is not full
        {
            Instruction opt;
            opt.fetch()
            res->push()
        }
};

#endif