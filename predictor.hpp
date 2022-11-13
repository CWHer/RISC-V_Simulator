#ifndef __PREDICTOR_HPP__
#define __PREDICTOR_HPP__

#include "RISC-V.h"
#include "counter2.hpp"

// Pattern History Table
class PatternHistoryTable
{
private:
    static const int N_BITS = 6;
    // assign a 2 bit counter to each address
    Counter2 buf[1 << N_BITS];
    int total_predict;

    unsigned hashAddr(unsigned addr)
    {
        static const unsigned high_offset = 8;
        static const unsigned high_mask = 0x7 << high_offset;
        static const unsigned low_mask = 0x7;
        unsigned hash_value = (addr & high_mask) >> high_offset + (addr & low_mask);
        assert(hash_value < (1 << N_BITS));
        return hash_value;
    }

public:
    PatternHistoryTable() : total_predict(0) {}

    int getTotalPredict() { return total_predict; }

    void update(unsigned addr, int is_jumped)
    {
        total_predict++;
        auto hash_value = hashAddr(addr);
        buf[hash_value].update(is_jumped);
    }

    bool predict(unsigned addr)
    {
        auto hash_value = hashAddr(addr);
        return buf[hash_value].predict();
    }
};

#endif