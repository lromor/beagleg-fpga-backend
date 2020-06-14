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

    // Transfer buffer to send, receive it into "receive" buffer. Both buffers
    // need to have the right siz.e
    bool TransferBuffer(const char *send, char *receive, size_t len);

    // Convenience version of method above
    bool TransferString(std::string &send, std::string *receive);
private:
    int fd_;
    struct Options options_;
};

#endif  // SPI_HOST_H
