#include"register.hpp"
#include"memory.hpp"
#include"predictor.hpp"

#include"issue.hpp"
#include"reservationstation.hpp"
#include"ALUnit.hpp"
#include"SLUnit.hpp"
#include"commondatabus.hpp"
#include"reorderbuffer.hpp"
#include<iostream>
Register reg;
Memory mem;
Predictor prd;
Issue IS(&mem,&reg,&prd);
ReservationStation res(&reg);
ALUnit ALU;
SLUnit SLU(&mem);
CommonDataBus CDB(&reg);
ReorderBuffer ROB(&mem,&reg,&prd);
void refresh()
{
    res.reset();
    ALU.reset();
    SLU.reset();
    CDB.reset();
    ROB.reset();
}
int main()
{
    // int cnt=0;
    // freopen("out","w",stdout);
    bool isRE=0;
    mem.init_read();
    while (!IS.empty()||!ROB.empty())
    {
        isRE=0;
        //deal with full condition inside run
        if (!ROB.stall()&&!IS.empty()) 
            IS.run(&res,&ROB);
        res.check(&ALU,&SLU);
        ALU.run();
        SLU.run();
        if (!ALU.isLock()&&!ALU.empty()) 
            CDB.push(ALU),ALU.reset();
        if (!SLU.isLock()&&!SLU.empty()) 
            CDB.push(SLU),SLU.reset();

        // std::cout<<CDB.size()<<std::endl;
        std::cout<<reg.getpc()<<std::endl;
        if (reg.getpc()==4484)
        {
            puts("1");
        }

        if (!CDB.empty()) CDB.run(&res,&ROB);
        if (!ROB.empty()) isRE=ROB.run();
        if (isRE) refresh();
        //debug
        // reg.printdata();
    }
    // std::cout<<cnt<<std::endl;
    int num=prd.tot-ROB.tot();
    printf("%d/%d %.2lf%\n",num,prd.tot,num*100.0/prd.tot);
    std::cout<<reg.output()<<std::endl;
    return 0;
}