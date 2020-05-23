#ifndef _SB_SPI_H
#define _SB_SPI_H

#include <stdint.h>
#include <vltstd/svdpi.h>

// Current default verilator.
#define VERILATOR_NEW 1

#include "common.h"

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

// A simulated SB_SPI IP System.
class SbSpiSecondaryMock : public SbSpiDpi::EventListener {
public:
  SbSpiSecondaryMock();
  virtual ~SbSpiSecondaryMock();
  void Trigger(const input_data *inputs, output_data *outputs) override;

  const input_data GetInput();
  const output_data GetOutput();

private:
  class Impl;
  std::unique_ptr<Impl> impl_;

};

#endif // SB_SPI_H_
