#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>

#include <vector>
#include <string>

// Convert an array of bits into an array of '0', '1' characters.
// The first character will be the LSB, the last the MSB.
// The parameter "data" contains the array of bytes of size at least
// num_bits / 8 + 1.
// "num_bits" is the amount of bits to convert into characters starting from LSB.
// "bit_sequence" must point to a char array of least "num_bits" characters.
// Return the number of characters written in "bit_sequence".
const size_t Bits2Chars(const uint8_t *data, const size_t num_bits,
                        char *bit_sequence);

// The inverse operation of Bits2Chars.
void Chars2Bits(const char *bit_sequence, const size_t num_bits, uint8_t *data);

// Higher level function that converts an array of null terminated strings(traces)
// into a sequence of parallel bits. The function supports up to 64 traces.
// The parameter "traces" is the array of null terminated strings.
// "num_traces" is the number of null terminated strings. "data" points
// to an array of at least size "data_elements".
// An example can be found in bits_test.cc
bool Traces2Bits(const char *traces[], const size_t num_traces, uint64_t *data,
                 const size_t data_elements);

// The inverse of Traces2Bits.
std::vector<std::string> Bits2Traces(const uint64_t *data, const size_t data_elements,
                                     const size_t num_traces);

#endif // _UTILS_H
