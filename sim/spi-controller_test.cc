
#include <iostream>

#include "verilator-generated/VSpiController.h"
#include "ice40-primitives/sb-spi.h"

// Callback from verilator calls our dpi controller method.
void sb_spi_dpi(const sb_spi_dpi_dtype* inputs, sb_spi_dpi_dtype* outputs) {
  SbSpiDpi::dpi_entrypoint((const input_data *) inputs, (output_data *) outputs);
}

class SbSpiTest : public SbSpiDpi::EventListener {
public:
  SbSpiTest() : inputs{0}, outputs{0} {}

  input_data inputs;
  output_data *outputs;

private:
  void trigger(const input_data *inputs, output_data *outputs) override {
    this->inputs = *inputs;
    this->outputs = outputs;
  }
};

int main(int argc, char *argv[]) {
  Verilated::commandArgs(argc, argv);
  VSpiController top_module = VSpiController();
  SbSpiTest sb_spi = SbSpiTest();

  SbSpiDpi::SetBackend(&sb_spi);

  for (int i = 0; i < 10; ++i) {
    top_module.clki ^= 1;
    top_module.eval();
    std::cout << (int) sb_spi.inputs.bits.sbclki << std::endl;
  }

  return 0;
}
