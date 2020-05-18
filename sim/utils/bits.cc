
#include<stdio.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

#include <vector>
#include <string>
#include <sstream>

#include "utils.h"

inline char bit2char(const uint8_t value, const unsigned int pos) {
  return ((value >> pos) & 1) ? '1' : '0';
}

inline uint8_t char2bit(const char value, const unsigned int pos,
                        const uint8_t out) {
  const uint8_t bit = (1 << pos);
  return (value == '1') ? out | bit : out & ~bit;
}

void Bits2Chars(const uint8_t *data, const size_t num_bits, char *parallel_bits) {
  for (unsigned int i = 0; i < num_bits; ++i) {
    const unsigned int byte = i / 8;
    const unsigned int bit = i % 8;
    parallel_bits[byte * 8 + bit] = bit2char(data[byte], bit);
  }
}

void Chars2Bits(const char *parallel_bits, const size_t num_bits, uint8_t *data){
  for (unsigned int i = 0; i < num_bits; ++i) {
    const unsigned int byte = i / 8;
    const unsigned int bit = i % 8;
    data[byte] = char2bit(parallel_bits[i], bit, data[byte]);
  }
}

bool Traces2Bits(const char *traces[], const size_t num_traces,
                 uint64_t *data, const size_t data_elements) {
  assert(num_traces <= 64);

  bool all_terminated;
  uint64_t *data_ptr = data;
  char *traces_ptr[num_traces];
  char *trace_ptr;

  // Characters mapped to '0'/'1'
  char parallel_bits[num_traces];

  // Initialize parallel bits to '0'.
  memset(parallel_bits, '0', num_traces);

  // Copy the char traces pointers
  memcpy(&traces_ptr, traces, sizeof(char *) * num_traces);

  // Iterate through each time step of the traces.
  for (unsigned int i = 0; i < data_elements + 1; ++i) {
    all_terminated = true;
   
    for (unsigned int j = 0; j < num_traces; ++j) {
      trace_ptr = traces_ptr[j];

      // Map '¯','-','1' to '1' and '_','0' to '0'.
      switch(*trace_ptr) {
      case '_':
      case '0':
        parallel_bits[j] = '0';
        break;
      case '\xc2':
        // Macron, skip a character
        traces_ptr[j]++;
        assert(*traces_ptr[j] == '\xaf');
      case '-':
      case '1':
        parallel_bits[j] = '1';
        break;
      case '\0':
        continue;
      default:
        fprintf(stderr, "Invalid character: '%c'(0x%x)\n", *trace_ptr, *trace_ptr);
        assert(false);
      }

      all_terminated = false;

      // Forward the trace of one time step.
      traces_ptr[j]++;
    }

    if (all_terminated)
      break;

    // Now we have an array of chars "010010".
    // Let's write the bits to our 64bit value.
    Chars2Bits(parallel_bits, num_traces, (uint8_t *)data_ptr++);
  }
  const unsigned int trace_length = (data_ptr - data);
  return (trace_length == data_elements) & all_terminated;
}

std::vector<std::string> Bits2Traces(const uint64_t *data, const size_t data_elements,
                                     const size_t num_traces) {
  char parallel_bits[num_traces];
  std::vector<std::string> traces(num_traces);

  // Iterate through every data elements
  for (unsigned int i = 0; i < data_elements; ++i) {
    // Convert to digit chars
    Bits2Chars((uint8_t *) &data[i], num_traces, parallel_bits);

    // Write low/high symbols.
    for (unsigned int j = 0; j < num_traces; ++j) {
      const char digit = parallel_bits[j];
      if (digit == '1') {
        traces[j].append("¯");
        } else {
        traces[j].append("_");
      }
    }
  }
  return traces;
}
