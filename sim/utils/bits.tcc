
#include<stdio.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

#include <vector>
#include <string>
#include <sstream>

#include "utils.h"

// Return a string given an array of bytes and a size corresponding
// to the total number of bits to print.
std::string Bits2Chars(const uint8_t *bits, size_t nbits,
                       const char zero, const char one) {
  const unsigned int bytes = nbits / 8;
  const unsigned int rem_bits = nbits % 8;
  std::stringstream ss;

  for (unsigned i = 0; i < bytes; ++i) {
    for (unsigned j = 0; j < 8; ++j) {
      const char value = ((bits[i] >> j) & 1) ? one : zero;
      ss << value;
    }
  }

  for (unsigned i = 0; i < rem_bits; ++i) {
    const char value = ((bits[i] >> i) & 1) ? one : zero;
    ss << value;
  }

  return ss.str();
}


// Supports dashes(-) and macron characters (¯)
// The input should be an array of null terminated strings.
// Each of these strings can be called "traces".
// If a trace is longer, the final vector of bits will be cropped
// to the shortest trace. If the strings contains uknown characters
// they will be mapped to 0.
template<typename T, size_t size>
const std::vector<T> Chars2Bits(const char *(&data)[size]) {
  static_assert(CHAR_BIT == 8);
  const size_t out_byte_size = sizeof(T::data);
  static_assert(out_byte_size * 8 > size);
 
  std::vector<T> out;
  T t_data;
  uint8_t *bits = (uint8_t *)&t_data.data;

  // Iterate through each time step of the traces.
  for (;;) {
    // Initialize bits to zero..
    memset(bits, 0, out_byte_size);

    // Iterate through the bit fields.
    for (unsigned i = 0; i < size; ++i) {
      const char **pt = &data[i];
      const int byte_idx = i / 8;
      const int bit_idx = i % 8;
      switch(**pt) {
      default:
      case '_':
        bits[byte_idx] &= ~(1 << bit_idx);
        break;
      case '\xc2':
        // Macron, preemptively skip a character
        (*pt)++;
      case '-':
        bits[byte_idx]|= 1 << bit_idx;
        break;
      case '\0':
        *pt = NULL;
        break;
      }
      if (!*pt) goto end;
      // Move the associated pointer forward.
      (*pt)++;
    }
    out.push_back(t_data);
  }
end:
  return out;
};
