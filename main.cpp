#include "simulator.hpp"
#include "memory.hpp"

int main()
{
    // freopen("output.log", "w", stdout);
    Memory memory;
    memory.initMemory();
    // memory.printMemory();
    Simulator simulator(&memory);
    simulator.setIntIndex(-1);
    simulator.run();
    simulator.printStatics();
    std::cout << simulator.getResult() << std::endl;
    return 0;
}