#ifndef SB_SPI_H_
#define SB_SPI_H_

#include <vltstd/svdpi.h>

#include "common.h"

union input_data {
  uint32_t data;

  // Reversed bit order due to inverted endianess
  // https://github.com/verilator/verilator/issues/1191
  struct {
    uint8_t scsni : 1; // Secondary chip select.
    uint8_t scki : 1; // Secondary clock inputs.
    uint8_t si : 1; // secondary input.
    uint8_t mi : 1; // Main input.
    uint8_t sbdati : 8; // System bus data input.
    uint8_t sbadri : 8; // System bus adress.
    uint8_t sbstbi : 1; // Strobe.
    uint8_t sbrwi : 1; // Read/Write mode bit.
    uint8_t sbclki : 1; // Clock.
  } __attribute__((packed)) bits;
};

union output_data {
  uint32_t data;

  struct {
    uint8_t mcsnoe : 4; // Main chip select output enable.
    uint8_t mcsno : 4; // Main chip select output.
    uint8_t sckoe :1; // Secondary clock enable.
    uint8_t scko :1; // Secondary clock output.
    uint8_t moe :1; // Main output enable.
    uint8_t mo : 1; // Main output.
    uint8_t soe : 1; // Secondary output enable.
    uint8_t so : 1; // Secondary output.
    uint8_t spiwkup : 1; // Wake up.
    uint8_t spiirq : 1; // IRQ
    uint8_t sbacko : 1; // ACK
    uint8_t sbdato : 8; // System bus data output.
  } __attribute__((packed)) bits;
};

// Define the static class delegated to manage the dpi.
// It's possible to register a backend for the dpi by inheriting
// the SbSpiDpi::EventListener and registering the backend with
// SbSpiDpi::RegisterBackend(my_event_listener);
typedef DpiController<input_data, output_data> SbSpiDpi;

extern "C" {
#ifdef VERILATOR_NEW
  typedef svLogicVecVal sb_spi_dpi_dtype;
#else
  typedef svBitVecVal sb_spi_dpi_dtype;
#endif

  void sb_spi_dpi(const sb_spi_dpi_dtype* inputs, sb_spi_dpi_dtype* outputs);
}

#endif // SB_SPI_H_
