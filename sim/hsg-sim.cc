// Simulator of the hardware step generator module.
#include "verilator-generated/top-module.h"
#include <bits/stdint-uintn.h>
#include <verilated.h>
#include "hsg-sim.h"
#include <unistd.h>

size_t StepGeneratorModuleSim::SendReceive(void *data, size_t size, void *received) {

  unsigned nbits = size * 8;
  uint8_t *send = (uint8_t *) data;
  uint8_t *recv = (uint8_t *) received;

  // Transmit every bit
  for (size_t i = 0; i < nbits; ++i) {
    const uint8_t byte = send[i / 8];
    const short bit_index = (i % 8);
    top_.spi_mosi = byte & (1 << bit_index);
    Cycle();
    recv[byte] |= (top_.spi_miso << bit_index);
  }

  return size;
}
