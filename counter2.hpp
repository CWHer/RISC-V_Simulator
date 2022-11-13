#ifndef __COUNTER2_HPP__
#define __COUNTER2_HPP__

#include "RISC-V.h"

class Counter2
{
    // clang-format off
    enum States
    {
        ST, WT, WNT, SNT
        // NOTE: Strongly / Weakly / Not take / Take
    };
    // clang-format on

private:
    States state;

public:
    Counter2() : state(WT) {}

    void update(int is_jumped)
    {
        if ((state == SNT && is_jumped == 1) ||
            (state == ST && is_jumped == -1))
            return;
        state = (States)((int)state + is_jumped);
    }

    bool predict()
    {
        switch (state)
        {
        case ST:
        case WT:
            return true;
        case WNT:
        case SNT:
            return false;
        }
    }
};

#endif