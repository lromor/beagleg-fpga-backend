// Simulator of the hardware step generator module.
#include "verilator-generated/top-module.h"
#include <bits/stdint-uintn.h>
#include <verilated.h>
#include "hsg-sim.h"
#include <unistd.h>
#include <iostream>

bool StepGeneratorModuleSim::SendReceive(const void *send, void *receive, size_t len,
                                         bool is_last_in_transaction) {

  const unsigned nbits = len * 8;
  uint8_t *csend = (uint8_t *) send;
  uint8_t *crecv = (uint8_t *) receive;

  memset(receive, 0, len);
  if (top_.spi_cs != 0) {
    top_.spi_cs = 0;
    Cycle(5);
  }

  // Transmit every bit
  for (size_t i = 0; i < nbits; ++i) {
    const int byte_index = i / 8;
    const uint8_t byte = csend[byte_index];
    const int bit_index = 7 - (i % 8);
    const uint8_t bit = (byte & (1 << bit_index)) ? 1 : 0;
    top_.spi_mosi = bit;
    Cycle(2);
    top_.spi_sck = 1;
    Cycle(5);
    const uint8_t miso = top_.spi_miso;
    Cycle(5);
    top_.spi_sck = 0;
    crecv[byte_index] |= (miso << bit_index);
  }

  Cycle(5);

  if (is_last_in_transaction) {
    top_.spi_cs = 1;
    Cycle(100);
  }
  return true;
}
