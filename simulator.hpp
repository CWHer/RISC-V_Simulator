#include "RISC-V.h"
#include "register_file.hpp"
#include "memory.hpp"
#include "predictor.hpp"
#include "issue.hpp"
#include "reservation_station.hpp"
#include "alu.hpp"
#include "slu.hpp"
#include "common_databus.hpp"
#include "reorder_buffer.hpp"

class Simulator
{
private:
    int total_cycles;
    RegisterFile reg_file;
    Memory *memory;
    PatternHistoryTable predictor;
    Issue inst_issue;
    ReservationStation res_station;
    ALUnit alu;
    SLUnit slu;
    CommonDataBus data_bus;
    ReorderBuffer reorder_buf;

    void resetAll()
    {
        reg_file.resetDep();
        inst_issue.reset();
        res_station.reset();
        alu.reset();
        slu.reset();
        data_bus.reset();
        reorder_buf.reset();
    }

    void runAll()
    {
        bool is_wrong = false;
        while (!inst_issue.isDone() || !reorder_buf.empty())
        {
            total_cycles++;
            // std::cout << std::dec << "Cycle " << total_cycles << std::endl;

            is_wrong = false;
            static const int NUM_ISSUE = 4;
            // Issue --> Reservation Station
            for (int i = 0; i < NUM_ISSUE; i++)
                if (!reorder_buf.is_stalled() && !inst_issue.isDone())
                    inst_issue.issueInst(&res_station, &reorder_buf);
            // reorder_buf.printEntry();

            // Reservation Station --> ALU / SLU
            res_station.sendReadyEntry(&alu, &slu);
            // res_station.printEntry();
            // clang-format off
            if (!alu.empty()) alu.run();
            if (!slu.empty()) slu.run();
            // clang-format on
            // alu.printEntry();
            // slu.printEntry();
            // std::cout << std::endl;

            // ALU / SLU --> Common Data Bus
            if (!alu.isBusy() && !alu.empty())
                data_bus.push(alu), alu.reset();
            if (!slu.isBusy() && !slu.empty())
                data_bus.push(slu), slu.reset();

            // Common Data Bus --> Reorder Buffer
            while (!data_bus.empty())
                data_bus.broadcast(&res_station, &reorder_buf);

            // commit instructions
            while (!reorder_buf.empty() &&
                   reorder_buf.isReady() && !is_wrong)
                is_wrong = reorder_buf.commitInst();

            // reorder_buf.printEntry();
            // res_station.printEntry();
            // reg_file.printRegfile();

            if (is_wrong)
            {
                resetAll();
                // std::cout << "==========Reset==========" << std::endl;
            }
        }
    }

public:
    Simulator(Memory *memory)
        : total_cycles(0), memory(memory),
          inst_issue(memory, &reg_file, &predictor),
          res_station(&reg_file), slu(memory),
          reorder_buf(memory, &reg_file, &predictor) {}

    void run() { runAll(); }

    unsigned getResult() { return reg_file.getResult(); }

    void printStatics()
    {
        std::cout << "Total cycles: " << total_cycles << std::endl;
        int total_predict = predictor.getTotalPredict();
        int correct_predict = total_predict - reorder_buf.errorNum();
        std::cout << "Predictor accuracy: " << correct_predict << "/" << total_predict << " ("
                  << (double)correct_predict / total_predict * 100 << "%)" << std::endl;
    }
    void setIntIndex(unsigned index)
    {
        reorder_buf.setIntIndex(index);
    }
};
