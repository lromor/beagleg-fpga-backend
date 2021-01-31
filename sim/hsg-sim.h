#ifndef __HSG_SIM_H_
#define __HSG_SIM_H_

#include "verilator-generated/top-module.h"
#include <verilated.h>

class StepGeneratorModuleSim {
public:
  // Used to initialize the verilator simulation.
  // At some point we might want to setup some threads etc..
  static StepGeneratorModuleSim *Init(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    return new StepGeneratorModuleSim();
  }

  // Takes care of sending size bytes of data.
  // The received payload is pointed from *out.
  // The size of the received payload is returned.
  bool SendReceive(const void *send, void *receive, size_t len,
                   bool is_last_in_transaction = true);

  void Cycle(int n = 1) {
    for (int i = 0; i < n; ++i) {
      top_.clk = ~top_.clk;
      top_.eval();
      top_.clk = ~top_.clk;
      top_.eval();
    }
  }

private:
  StepGeneratorModuleSim() {
    top_.spi_cs = 1;
    Cycle();
  }
  VTop top_;
};


#endif // __HSG_SIM_H_
