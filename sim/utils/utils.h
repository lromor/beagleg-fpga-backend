#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>

#include <vector>
#include <string>

template<typename T, size_t size>
const std::vector<T> Chars2Bits(const char *(&data)[size]);

template<char zero = '0', char one = '1'>
std::string Bits2Chars(const uint8_t *bits, size_t nbits);

// Template function implementations.
#include "bits.tcc"

#endif // _UTILS_H
