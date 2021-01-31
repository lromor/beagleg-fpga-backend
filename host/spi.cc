#include "spi.h"

#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>

// Report io-error, return false.
static bool io_problem(const char *msg) {
    perror(msg);
    return false;
}

static void hex_dump(const void *src, size_t length, const char *prefix) {
    const uint8_t *address = (const unsigned char*) src;

    printf("%s | ", prefix);
    if (!src) {
        printf("\e[1;31m(ignored)\e[0m\n");
        return;
    }
    if (length == 0) {
        printf("\e[1;31m(no bytes)\e[0m\n");
        return;
    }
    for (size_t b = 0; b < length; ++b) {
        printf("%02X ", address[b]);
    }
    printf(" | ");
    for (size_t b = 0; b < length; ++b) {
        uint8_t c = address[b];
        printf("%c", (c < 33 || c == 255) ? '.' : c);
    }
    printf("\e[0m\n");
}

SPIHost::~SPIHost() {}

bool SPIHost::Connect(const char *device, const Options &set_options) {
    return true;
}


bool SPIHost::TransferBuffer(const void *send, void *receive, size_t len,
                             bool is_last_in_transaction) {
    m_->SendReceive(send, receive, len, is_last_in_transaction);
    hex_dump(send, len, "\t\e[1;36mTX");
    hex_dump(receive, len, "\t\e[1;33mRX");
    return true;
}
