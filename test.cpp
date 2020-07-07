#include"instruction.hpp"
#include"execute.hpp"
#include"register.hpp"
#include"memory.hpp"
Register reg;
Instruction opt;
Memory mem;
int main()
{
    mem.init_read();
    // freopen("in","r",stdin);
    while (1)
    {
        opt.fetch(&mem,&reg);
        opt.decode();
        Execute exe(&opt,&reg,&mem);
        exe.run();
        exe.memory_access();
        exe.write_back();
        // t.read();
        // t.decode();
        // Execute e(&t,&reg);
        // e.run(); 
    }
    return 0;
}