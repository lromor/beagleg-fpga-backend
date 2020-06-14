// -*- c++ -*-

#ifndef SPI_HOST_H
#define SPI_HOST_H

#include <stdint.h>
#include <string>

class SPIHost {
public:
    struct Options {
        uint32_t mode;
        uint8_t bits_per_word = 8;
        uint32_t speed_hz = 500000;
    };

    SPIHost() : fd_(-1) {}

    // Connect to given device, e.g. "/dev/spidev0.0".
    bool Connect(const char *device, const Options &options);

    // Send string with, put received data in miso.
    bool Transfer(std::string &mosi, std::string *miso);

private:
    int fd_;
    struct Options options_;
};

#endif  // SPI_HOST_H
