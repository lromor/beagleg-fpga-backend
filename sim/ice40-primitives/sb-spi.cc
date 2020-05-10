
#include "sb-spi.h"

#define SPICR0_ADDR 0b1000 // SPI Control Register 0
#define SPICR1_ADDR 0b1001 // SPI Control Register 1
#define SPICR2_ADDR 0b1010 // SPI Control Register 2
#define SPIBR_ADDR 0b1011 // SPI Baud Rate Register
#define SPITXDR_ADDR 0b1101 // SPI Transmit Data Register
#define SPIRXDR_ADDR 0b1110 // SPI Receive Data Register
#define SPICSR_ADDR 0b1111 // SPI Chip Select Mask
#define SPISR_ADDR 0b1100 // SPI Status Register
#define SPIINTSR_ADDR 0b0110 // SPI Interrupt Status Register
#define SPIINTCR_ADDR 0b0111 // SPI Interrupt Control Register

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

class SbSpiMock::Impl {
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

void SbSpiMock::Impl::StepLogic(const input_data *inputs, output_data *outputs) {
  // If clock is
  // New input!
  // switch (state_) {
  //   case IDLE:
  // }
}

const input_data SbSpiMock::GetInput() { return impl_->GetInput(); }

const output_data SbSpiMock::GetOutput() { return impl_->GetOutput(); }

void SbSpiMock::trigger(const input_data *inputs, output_data *outputs) {
  impl_->StepLogic(inputs, outputs);
}

SbSpiMock::SbSpiMock() : impl_(new Impl) {}
SbSpiMock::~SbSpiMock() = default;
