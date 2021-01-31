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
  top_.spi_cs = 0;
  Cycle(100);

  // Transmit every bit
  for (size_t i = 0; i < nbits; ++i) {
    const int byte_index = i / 8;
    const uint8_t byte = csend[byte_index];
    const int bit_index = (i % 8);
    top_.spi_mosi = byte & (1 << bit_index);
    top_.spi_sck = 1;
    Cycle(200);
    top_.spi_sck = 0;
    Cycle(200);
    const uint8_t miso = top_.spi_miso;
    printf("miso: %d, bit_index: %d, byte: %d\n", miso, bit_index, byte_index);
    crecv[byte_index] |= (miso << (7 - bit_index));
  }
  Cycle(100);
  top_.spi_cs = (is_last_in_transaction) ? 1 : 0;
  Cycle(100);

  for (size_t i = 0; i < len; ++i) {
    std::cout << std::hex << (int) crecv[i];
  }
  std::cout << "\n";
  return true;
}
