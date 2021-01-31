#ifndef __HSG_SIM_H_
#define __HSG_SIM_H_

#include "verilator-generated/top-module.h"
#include <verilated.h>
#include <verilated_vcd_c.h>

class StepGeneratorModuleSim {
public:
  // Used to initialize the verilator simulation.
  // At some point we might want to setup some threads etc..
  static StepGeneratorModuleSim *Init(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    Verilated::traceEverOn(true);
    return new StepGeneratorModuleSim();
  }

  // Takes care of sending size bytes of data.
  // The received payload is pointed from *out.
  // The size of the received payload is returned.
  bool SendReceive(const void *send, void *receive, size_t len,
                   bool is_last_in_transaction = true);

  void Tick() {
    top_.clk = 0;
    top_.eval();
    trace_.dump(10 * ticks_);
    top_.clk = 1;
    top_.eval();
    trace_.dump(10 * ticks_ + 5);
    ticks_++;
    trace_.flush();
  }

  void Cycle(unsigned n = 1) {
    for (unsigned i = 0; i < n; ++i)
      Tick();
  }

private:
  StepGeneratorModuleSim() : ticks_(0) {
    // Trace 99 levels of hierarchy
    top_.trace(&trace_, 99);
    trace_.open("trace.vcd");
    top_.spi_cs = 1;
    Tick();
  }

  VerilatedVcdC trace_;
  VTop top_;
  unsigned long ticks_;
};


#endif // __HSG_SIM_H_
