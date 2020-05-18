
#include <iostream>
#include <stdint.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "utils.h"

static const char *sample_trace[] = {
  "", // Empty trace
  "¯", // Start as high
  "¯_¯0__¯¯1¯¯__¯¯__1¯¯¯__¯11__¯¯_0", // Mixed trace
  "_¯1_¯¯__¯-¯_0--_¯¯__0¯__¯_", // Shorter mixed.
  "k2jhekj12h" // invalid
};

static const std::vector<std::string> sample_expected_trace = {
  {"________________________________"},
  {"¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯"},
  {"¯_¯___¯¯¯¯¯__¯¯__¯¯¯¯__¯¯¯__¯¯__"},
  {"_¯¯_¯¯__¯¯¯__¯¯_¯¯___¯__¯_______"},
};

TEST(Bits2Chars2BitsTest, back_and_forth_conversion) {
  uint64_t sample = 0xc6ed41bfa0b47df0;
  uint64_t mask = 0;
  char parallel_bits[64] = {0};
  uint64_t result;

  for (unsigned int i = 0; i < 64; ++i) {
    const uint64_t data = sample & mask;
    Bits2Chars((uint8_t *) &data, i, parallel_bits);
    result = 0;
    Chars2Bits(parallel_bits, i, (uint8_t *)&result);
    EXPECT_EQ(data, result);
    mask |= 1ULL << i;
  }
}

TEST(Traces2Bits, return_value_and_back_forth) {
  bool res;
  uint64_t data[32];
  // Not all traces reaching the null char.
  res = Traces2Bits(sample_trace, 4, data, 31);
  EXPECT_FALSE(res);

  // All traces reaching null char, wrong size
  res = Traces2Bits(sample_trace, 4, data, 33);
  EXPECT_FALSE(res);

  // All traces reaching null char, exact size provided.
  res = Traces2Bits(sample_trace, 4, data, 32);
  EXPECT_TRUE(res);

  auto trace = Bits2Traces(data, 32, 4);
  EXPECT_THAT(trace, ::testing::ContainerEq(sample_expected_trace));
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
