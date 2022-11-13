#ifndef __REGISTER_HPP__
#define __REGISTER_HPP__

#include "RISC-V.h"

class RegisterFile
{
private:
    static const int N_REG = 32;
    unsigned storage[N_REG];
    ROBEntry *entry_ptr[N_REG];
    unsigned program_counter;

public:
    RegisterFile()
    {
        program_counter = 0;
        std::memset(storage, 0, sizeof(storage));
        std::memset(entry_ptr, 0, sizeof(entry_ptr));
    }

    void write(unsigned pos, unsigned val)
    {
        storage[pos] = val;
    }

    // dependency utils
    void resetDep()
    {
        std::memset(entry_ptr, 0, sizeof(entry_ptr));
    }

    void setDep(unsigned pos, ROBEntry *ptr)
    {
        assert(pos > 0 && pos < N_REG);
        entry_ptr[pos] = ptr;
    }

    ROBEntry *getDep(unsigned pos)
    {
        return entry_ptr[pos];
    }

    unsigned setPC(unsigned val)
    {
        program_counter = val;
    }

    void nextPC()
    {
        static const unsigned INST_LEN = 4;
        program_counter += INST_LEN;
    }

    unsigned getResult()
    {
        // HACK: result is stored in reg[10]
        return storage[10] & 0xff;
    }

    void printRegfile()
    {
        for (int i = 0; i < N_REG; ++i)
            std::cout << "reg[" << i << "] = " << storage[i] << '\n';
        std::cout << std::endl;
    }
};
#endif