
#include "verilator-generated/VSB_SPI.h"
#include "sb-spi.h"
#include "../utils/utils.h"

class SbSpiMain {
public:
  SbSpiMain(const VSB_SPI *sb_spi_module) : module_(sb_spi_module) {}

  SbSend(const uint32_t data) {
    const input_data input = { data };
    top_module.SBCLKI ^= 1;
    top_module.eval();
  }

private:
  VSB_SPI *module_;
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

  Verilated::commandArgs(argc, argv);
  VSB_SPI sb_spi_module = VSB_SPI();
  SbSpiMock sb_spi_backend = SbSpiMock();
  SbSpiDpi::SetBackend(&sb_spi_backend);

  SbSpiMain main = SbSpiMain(sb_spi_module);
  for (auto i : Traces2Bits(secondary_input)) {
    main.Send(i)
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
