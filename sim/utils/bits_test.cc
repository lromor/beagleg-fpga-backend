
#include <stdint.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "utils.h"

union input_data {
  uint8_t data[2];
  struct input {
    uint8_t b1 : 1;
    uint8_t b2 : 1;
  } bits;
};

// static const char *sample_dash[] = {
//   "-_-_-_",
//   "--_--_"
// };

// static const char *sample_macron[] = {
//   "¯_¯_¯_",
//   "¯¯_¯¯_"
// };

// static const std::vector<union input_data> samples_result = {
//   {{ 0b11 }}, {{ 0b01 }}, {{ 0b10 }}, {{ 0b01 }}, {{ 0b11 }}, {{ 0b00 }}
// };

// TEST(Chars2BitsTest, simple_dashed) {
//   auto res = Chars2Bits<input_data>(sample_dash);
//   EXPECT_THAT(samples_result, ::testing::ElementsAre(res));
// }

TEST(Chars2BitsTest, order) {
  const char *in[] = { "-_", "__" };
  auto res = Chars2Bits<input_data>(in);
  EXPECT_EQ(1, res[0].bits.b1);
  EXPECT_EQ(0, res[0].bits.b2);
  EXPECT_EQ(0, res[1].bits.b1);
  EXPECT_EQ(0, res[1].bits.b2);
}


int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
