#include"RISC-V.h"
#include"register.hpp"
#include"memory.hpp"
#include"predictor.hpp"

#include"issue.hpp"
#include"reservationstation.hpp"
#include"ALunit.hpp"
#include"SLunit.hpp"
#include"commondatabus.hpp"
#include"reorderbuffer.hpp"
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
    reg.resetQi();
    IS.reset();
    res.reset();
    ALU.reset();
    SLU.reset();
    CDB.reset();
    ROB.reset();
}
int main()
{
    int cnt=0;
    // freopen("out","w",stdout);
    // freopen("ans","w",stdout);
    ROB.setStopNum(150);
    bool isRE=0,isfull=0;
    mem.init_read();
    while (!IS.empty()||!ROB.empty())
    {
        ++cnt;
        isRE=isfull=0;
        //deal with full condition inside run
        // if (!ROB.stall()&&!IS.empty()) IS.run(&res,&ROB);

        while (!isfull&&!ROB.stall()&&!IS.empty()) 
            isfull=IS.run(&res,&ROB);
        
        res.check(&ALU,&SLU);
        ALU.run();
        SLU.run();
        if (!ALU.isLock()&&!ALU.empty()) 
            CDB.push(ALU),ALU.reset();
        if (!SLU.isLock()&&!SLU.empty()) 
            CDB.push(SLU),SLU.reset();

        // std::cout<<CDB.size()<<std::endl;
        // std::cout<<reg.getpc()<<std::endl;
        // if (cnt>520) break;
        // if (reg.getpc()==4472)
        // {
        //     puts("1");
        // }

        while (!CDB.empty()&&CDB.isReady(&ROB)) 
            CDB.run(&res,&ROB);
        while (!ROB.empty()&&ROB.isReady()&&!isRE) 
            isRE=ROB.run();
        if (isRE) refresh();
        //debug
        // reg.printdata();
    }
    std::cout<<cnt<<std::endl;
    int num=prd.tot-ROB.tot();
    printf("%d/%d %.2lf%\n",num,prd.tot,num*100.0/prd.tot);
    std::cout<<reg.output()<<std::endl;
    return 0;
}