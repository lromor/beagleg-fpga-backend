
#include <vltstd/svdpi.h>

#include "common.h"

struct sb_spi_inputs {
  uint32_t ports;
};

struct sb_spi_outputs {
  uint32_t ports;
};

// Need to reverse bits...
// https://github.com/verilator/verilator/issues/1191
struct sb_spi_inputs_fields {
  uint8_t slave_chip_select : 1;
  uint8_t slave_clock_input : 1;
  uint8_t slave_input : 1;
  uint8_t master_input : 1;
  uint8_t data_input : 8;
  uint8_t address : 8;
  uint8_t strobe : 1;
  uint8_t rw : 1;
  uint8_t clk : 1;
} __attribute__((packed));


struct sb_spi_outputs_fields {
  uint8_t master_chip_select_output_enable : 4;
  uint8_t master_chip_select_output : 4;
  uint8_t slave_clock_enable :1;
  uint8_t slave_clock_output :1;
  uint8_t master_output_enable :1;
  uint8_t master_output : 1;
  uint8_t slave_output_enable : 1;
  uint8_t slave_output : 1;
  uint8_t wake_up : 1;
  uint8_t irq : 1;
  uint8_t ack : 1;
  uint8_t data_output : 1;
} __attribute__((packed));

union input_data {
  struct sb_spi_inputs data;
  struct sb_spi_inputs_fields repr;
};

union output_data {
  struct sb_spi_outputs *data;
  struct sb_spi_outputs_fields *repr;
};

// Define the static class delegated to manage the dpi.
// It's possible to register a backend for the dpi by inheriting
// the SbSpiDpi::EventListener and registering the backend with
// SbSpiDpi::RegisterBackend(my_event_listener);
// In this case the input structs are just made of 32 bit values but bigger
// datatypes are also suppoert which are compatible with C/C++ justyfing the
// need of specifying a specific struct for a specific DPI.
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
