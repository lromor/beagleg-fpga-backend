#ifndef __HSG_SIM_H_
#define __HSG_SIM_H_

#include "verilator-generated/top-module.h"
#include <verilated.h>

class StepGeneratorModuleSim {
public:
  // Used to initialize the verilator simulation.
  // At some point we might want to setup some threads etc..
  StepGeneratorModuleSim *Init(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    return new StepGeneratorModuleSim();
  }

  // Takes care of sending size bytes of data.
  // The received payload is pointed from *out.
  // The size of the received payload is returned.
  size_t SendReceive(void *data, size_t size, void *received);

  void Cycle() {
    top_.clk = ~top_.clk;
    top_.eval();
  }

private:
  StepGeneratorModuleSim() {}
  VTop top_;
};


#endif // __HSG_SIM_H_
