#include"instruction.hpp"
#include"execute.hpp"
Register reg;
Instruction t;

int main()
{
    freopen("in","r",stdin);
    while (1)
    {
        t.read();
        t.decode();
        Execute e(&t,&reg);
    }
    return 0;
}