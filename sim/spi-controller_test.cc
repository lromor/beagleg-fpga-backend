
#include <iostream>

#include "verilator-generated/VSpiController.h"
#include "ice40-primitives/sb-spi.h"

int main(int argc, char *argv[]) {
  Verilated::commandArgs(argc, argv);
  VSpiController top_module = VSpiController();
  SbSpiMock sb_spi = SbSpiMock();

  SbSpiDpi::SetBackend(&sb_spi);

  for (int i = 0; i < 10; ++i) {
    top_module.sb_clk_i ^= 1;
    top_module.eval();
  }

  return 0;
}
