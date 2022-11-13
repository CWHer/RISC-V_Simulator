#ifndef __COMMON_DATABUS_HPP__
#define __COMMON_DATABUS_HPP__

#include "RISC-V.h"
#include "executable.hpp"
#include "register_file.hpp"
#include "reservation_station.hpp"
#include "alu.hpp"
#include "slu.hpp"
#include "reorder_buffer.hpp"

class CommonDataBus
{
private:
    std::queue<ExecWarp *> inst_queue;

public:
    void push(SLUnit SLU) { inst_queue.push(SLU.executable); }
    void push(ALUnit ALU) { inst_queue.push(ALU.executable); }

    void reset()
    {
        while (!inst_queue.empty())
            inst_queue.pop();
    }

    void broadcast(ReservationStation *res_station,
                   ReorderBuffer *reorder_buf)
    {
        ExecWarp *executable = inst_queue.front();
        inst_queue.pop();
        res_station->broadcastOperand(executable->entry.dest,
                                      executable->reg_result);
        reorder_buf->update(executable);
    }

    bool empty()
    {
        return inst_queue.empty();
    }
};

#endif