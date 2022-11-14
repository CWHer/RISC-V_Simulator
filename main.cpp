#include "simulator.hpp"
#include "memory.hpp"

int main()
{
    // freopen("output.log", "w", stdout);
    Memory memory;
    memory.initMemory();
    // memory.printMemory();

    Simulator simulator(&memory);
    simulator.setMode(Simulator::Mode::SERIAL);
    simulator.run();
    simulator.printStatics();
    std::cout << std::dec << simulator.getResult() << std::endl;
    return 0;
}