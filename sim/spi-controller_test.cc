
#include <iostream>

#include "verilated/VSpiController.h"
#include "ice40-primitives/sb-spi.h"


static void print_sb_spi_values(const struct sb_spi_inputs *inputs, struct sb_spi_outputs *outputs) {
  // We need to map correctly the struct passed via DPI and here printed.
  std::cout << *(int *)inputs << std::endl;
  std::cout << *(int *)outputs << std::endl;
}


class SbSpiTest : public SbSpiMock {
public:
  virtual ~SbSpiTest() {}

private:
  virtual void trigger(const struct sb_spi_inputs *inputs, struct sb_spi_outputs *outputs) override {
    print_sb_spi_values(inputs, outputs);
  }
};

int main(int argc, char *argv[]) {
  Verilated::commandArgs(argc, argv);
  VSpiController *top_module = new VSpiController();
  SbSpiTest *sb_spi = new SbSpiTest();

  SbSpiDpi::SetBackend(sb_spi);

  for (int i = 0; i < 10; ++i) {
    top_module->clki ^= 1;
    top_module->eval();
    break;
  }

  delete top_module;
  delete sb_spi;
  return 0;
}
