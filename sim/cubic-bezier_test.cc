#include <ios>
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <iomanip>
#include "verilated.h"
#include "verilator-generated/xls/cubic-bezier-module.h"
#include "fpm/fixed.h"
#include "fpm/ios.h"

#define MAX_X 1 << 16

std::string data2hex(void *data, size_t size_bytes) {
  std::stringstream ss;
  char *byte_data = (char *) data;
  ss << "0x";
  for (; size_bytes > 0; --size_bytes) {
    ss << std::setfill('0') << std::setw(2) << std::right << std::hex << static_cast<int>(static_cast<unsigned char>(byte_data[size_bytes - 1]));
  }
  return ss.str();
}

static inline int32_t unsigned2signed(uint32_t value, size_t bits) {
  const int sign = value & (1 << (bits - 1)) ? -1 : 1;
  const int unsigned_value = (sign > 0) ? value : (~value + 1) & ((1 << (bits)) - 1);
  return unsigned_value * sign;
}

namespace fpm {
  using fixed_30_2 = fixed<std::int32_t, std::int64_t, 2>;
}

// It looks like it takes 4 cycles to compute the result.
static inline void step_sim(VCubicBezier *tb) {
  for (int i = 0; i < 8; ++i) {
    tb->clk = ~tb->clk;
    tb->eval();
  }
}

int main(int argc, char *argv[]) {
  // Create an instance of our module under test
  VCubicBezier *tb = new VCubicBezier();

  double kC1 = 28;
  double kC2 = -2.5;
  double kC3 = 1.0;

  double kA = 3 * kC1; // width: 16 + 2
  double kB = 3 * (kC2 - 2 * kC1); // width = 16 + 1 + 3
  double kC = 3 * kC1 - 3 * kC2 + kC3; // width = 19 + 1

  // Sample control points
  fpm::fixed_30_2 a{kA};
  fpm::fixed_30_2 b{kB};
  fpm::fixed_30_2 c{kC};

  tb->a = a.raw_value();
  tb->b = b.raw_value();
  tb->c = c.raw_value();
  tb->clk = 0;

  for (unsigned i = 0; i < MAX_X; ++i) {
    tb->x = i;
    step_sim(tb);
    std::cout << data2hex(&tb->x, 3) << " " << data2hex(tb->out, 10) << std::endl;
  }

  return 0;
}
