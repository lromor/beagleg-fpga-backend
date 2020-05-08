
#include <iostream>

#include "verilated/VSpiController.h"
#include "ice40-primitives/sb-spi.h"

class SbSpiTest : public SbSpiMock {
public:
  SbSpiTest() : inputs({0}), outputs({0}) {}
  virtual ~SbSpiTest() {}

  union input_data inputs;
  union output_data outputs;
private:
  virtual void trigger(const struct sb_spi_inputs *inputs, struct sb_spi_outputs *outputs) override {
    this->inputs.data = *inputs;
    this->outputs.data = outputs;
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
    std::cout << (int) sb_spi->inputs.repr.clk << std::endl;
  }

  delete top_module;
  delete sb_spi;
  return 0;
}
