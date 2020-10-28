#include <ios>
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include "utils/fpm/fixed.h"
#include "utils/fpm/ios.h"
#include <iostream>
#include <iomanip>
#include "verilated.h"
#include "verilator-generated/cubic-bezier-module.h"

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

void print_x(VCubicBezier *tb) {
  std::stringstream ss;
  ss << "{ ";
  ss << "s: " << std::right << std::setw(6) << tb->s << ", ";
  ss << "s2: " << std::right << std::setw(10) << tb->cubic_bezier__DOT__s2 << ", ";
  ss << "s3: " << std::right << std::setw(20) << tb->cubic_bezier__DOT__s3 << ", ";
  ss << "oms: " << std::right << std::setw(6) << tb->cubic_bezier__DOT__oms << ", ";
  ss << "oms2: " << std::right << std::setw(10) << tb->cubic_bezier__DOT__oms2 << ", ";
  ss << "p1x: " << std::right << std::setw(7) << tb->p1x << ", ";
  ss << "p2x: " << std::right << std::setw(7) << tb->p2x << ", ";
  ss << "p3x " << std::right << std::setw(7) << tb->p3x << ", ";
  ss << "stepsx" << std::right << std::setw(10) << unsigned2signed(tb->stepsx, 18) << ", ";
  ss << "resx" << std::right << std::setw(30)
     << data2hex((void *)tb->cubic_bezier__DOT__result[0], sizeof(tb->cubic_bezier__DOT__result[0]));

  ss << " }";
  ss << std::endl;
  std::cout << ss.str();
}

int main(int argc, char *argv[]) {
  // Create an instance of our module under test
  VCubicBezier *tb = new VCubicBezier();
  fpm::fixed_16_16 p1[3] = {fpm::fixed_16_16{-4223}, fpm::fixed_16_16{878}, fpm::fixed_16_16{540}};
  fpm::fixed_16_16 p2[3] = {fpm::fixed_16_16{180}, fpm::fixed_16_16{-2390}, fpm::fixed_16_16{-15}};
  fpm::fixed_16_16 p3[3] = {fpm::fixed_16_16{-1238}, fpm::fixed_16_16{32}, fpm::fixed_16_16{-89}};

  tb->p1x = p1[0].raw_value();
  tb->p1y = p1[1].raw_value();
  tb->p1z = p1[2].raw_value();

  tb->p2x = p2[0].raw_value();
  tb->p2y = p2[1].raw_value();
  tb->p2z = p2[2].raw_value();

  tb->p3x = p3[0].raw_value();
  tb->p3y = p3[1].raw_value();
  tb->p3z = p3[2].raw_value();

  // Pass s from 0 (0) to 1 (1 << 16).
  // Notice that we don't actually reach the final points since the value
  // i = 65536 is never computed.
  for (unsigned i = 0; i < 65536; ++i) {
    tb->s = i;
    tb->eval();
    // print_x(tb);
    std::cout << i << " ";
    std::cout << unsigned2signed(tb->stepsx, 18) << " ";
    std::cout << unsigned2signed(tb->stepsy, 18) << " ";
    std::cout << unsigned2signed(tb->stepsz, 18) << " ";
    std::cout << std::endl;
  }
}
