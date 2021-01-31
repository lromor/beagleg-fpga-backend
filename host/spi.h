// -*- c++ -*-

#ifndef SPI_HOST_H
#define SPI_HOST_H

#include <cassert>
#include <stdint.h>
#include <string>
#include "../sim/hsg-sim.h"
#include <iostream>

class SPIHost {
public:
    struct Options {
        uint32_t mode;
        uint8_t bits_per_word = 8;
        uint32_t speed_hz = 500000;
        bool verbose = false;
    };

    SPIHost() : fd_(-1), m_(NULL) {
        m_ = StepGeneratorModuleSim::Init(0, NULL);
    }

    ~SPIHost();

    // Connect to given device, e.g. "/dev/spidev0.0".
    bool Connect(const char *device, const Options &options);

    // Transfer buffer to send, receive it into "receive" buffer. Both buffers
    // need to have the right size.
    // If is_last_in_transaction is false, keeps the CS low.
    bool TransferBuffer(const void *send, void *receive, size_t len,
                        bool is_last_in_transaction = true);

private:
    int fd_;
    struct Options options_;
    StepGeneratorModuleSim * m_;
};

#endif  // SPI_HOST_H
