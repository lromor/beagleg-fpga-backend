
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "verilator-generated/VSB_SPI.h"
#include "sb-spi.h"
#include "../utils/utils.h"


class SbSpiMain {
public:
  SbSpiMain(VSB_SPI * const sb_spi_module) : module_(sb_spi_module) {}

  void SbSend(const input_data data) {
    // Set the data
    // Toggle clock
    module_->SBCLKI ^= 1;

    // Evaluate
    module_->eval();
  }

private:
  VSB_SPI * const module_;
};

TEST(SbSpi, write_register) {
  // Feed this into the sb-spi
  const char *secondary_input[] = {
    "_¯_¯_¯_", // sbclki, 3 edges
    "_¯¯¯¯¯_", // sbcsi, strobe
    "_¯¯¯¯¯_", // sbcsi,
    "_¯¯¯¯¯_", // sbrwi, write mode
    "_______", // sbadri7
    "_______", // sbadri6
    "_______", // sbadri5
    "_______", // sbadri4
    "_______", // sbadri3
    "_______", // sbadri2
    "_______", // sbadri1
    "_______", // sbadri0
    "_¯¯¯¯¯_", // sbdati7
    "_¯¯¯¯¯_", // sbdati6
    "_¯¯¯¯¯_", // sbdati5
    "_¯¯¯¯¯_", // sbdati4
    "_¯¯¯¯¯_", // sbdati3
    "_¯¯¯¯¯_", // sbdati2
    "_¯¯¯¯¯_", // sbdati1
    "_¯¯¯¯¯_", // sbdati0
  };

  VSB_SPI sb_spi_module = VSB_SPI();
  SbSpiSecondaryMock sb_spi_backend = SbSpiSecondaryMock();
  SbSpiDpi::SetBackend(&sb_spi_backend);

  SbSpiMain main(&sb_spi_module);
  // Send the bits...
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
