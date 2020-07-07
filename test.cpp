#include"instruction.hpp"
#include"execute.hpp"
#include"register.hpp"
#include"memory.hpp"
Register reg;
Instruction t;
Memory mem;
int main()
{
    mem.init_read();
    // freopen("in","r",stdin);
    while (1)
    {
        mem.init_read();
        // t.read();
        // t.decode();
        // Execute e(&t,&reg);
        // e.run(); 
    }
    return 0;
}