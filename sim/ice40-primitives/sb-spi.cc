
#include "sb-spi.h"

// Callback from verilator calls our dpi controller method.
void sb_spi_dpi(const sb_spi_dpi_dtype* inputs, sb_spi_dpi_dtype* outputs) {
  SbSpiDpi::dpi_entrypoint((const input_data *) inputs, (output_data *) outputs);
}

struct sb_spi_registers {
  uint8_t spicr0;
  uint8_t spicr1;
  uint8_t spicr2;
  uint8_t spibr;
  uint8_t spitxdr;
  uint8_t spirxdr;
  uint8_t spicsr;
  uint8_t spisr;
  uint8_t spiintsr;
  uint8_t spiintcr;
};

enum state {
  IDLE,
  WRITE_START,
  WRITE_END,
  READ_START,
  READ_END
};

class SbSpiSecondaryMock::Impl {
public:

  Impl() : state_(IDLE), registers_{0} {}
  void StepLogic(const input_data *inputs, output_data *outputs);

  const input_data GetInput() { return input_; }
  const output_data GetOutput() { return output_; }

  void UpdateClock() {}


private:
  input_data input_;
  output_data output_;

  uint8_t clock; // Buffer of the last 8 clock values.

  state state_;
  sb_spi_registers registers_;
};

void SbSpiSecondaryMock::Impl::StepLogic(const input_data *inputs, output_data *outputs) {
  // If clock is
  // New input!
  // switch (state_) {
  //   case IDLE:
  // }
}

const input_data SbSpiSecondaryMock::GetInput() { return impl_->GetInput(); }

const output_data SbSpiSecondaryMock::GetOutput() { return impl_->GetOutput(); }

void SbSpiSecondaryMock::Trigger(const input_data *inputs, output_data *outputs) {
  impl_->StepLogic(inputs, outputs);
}

SbSpiSecondaryMock::SbSpiSecondaryMock() : impl_(new Impl) {}
SbSpiSecondaryMock::~SbSpiSecondaryMock() = default;
