
#include <vltstd/svdpi.h>

#include "common.h"

struct sb_spi_inputs {
  uint8_t clk;
  uint8_t rw;
  uint8_t strobe;
  uint8_t address;
  uint8_t data_input;
  uint8_t master_input;
  uint8_t slave_input;
  uint8_t slave_clock_input;
  uint8_t slave_chip_select;
};// __attribute__((packed));

struct sb_spi_outputs {
  uint8_t data_output;
  uint8_t ack;
  uint8_t irq;
  uint8_t wake_up;
  uint8_t slave_output;
  uint8_t slave_output_enable;
  uint8_t master_output;
  uint8_t master_output_enable;
  uint8_t slave_clock_output;
  uint8_t slave_clock_enable;
  uint8_t master_chip_select_output;
  uint8_t master_chip_select_output_enable;
};// __attribute__((packed));

// Define the static class delegated to manage the dpi.
// It's possible to register a backend for the dpi by inheriting
// the SbSpiDpi::EventListener and registering the backend with
// SbSpiDpi::RegisterBackend(my_event_listener);
typedef DpiController<struct sb_spi_inputs, struct sb_spi_outputs> SbSpiDpi;

// Callback from verilator calls our dpi controller method.
void sb_spi_dpi (const svBitVecVal* inputs, svBitVecVal* outputs) {
  SbSpiDpi::dpi_entrypoint((struct sb_spi_inputs*) inputs, (struct sb_spi_outputs *) outputs);
}

class SbSpiMock : public SbSpiDpi::EventListener {
public:
  SbSpiMock() {}
  virtual ~SbSpiMock() {}

private:
  virtual void trigger(const struct sb_spi_inputs *inputs, struct sb_spi_outputs *outputs) {}
};
