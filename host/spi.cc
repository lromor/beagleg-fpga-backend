#include "spi.h"

#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>

// Report io-error, return false.
static bool io_problem(const char *msg) {
    perror(msg);
    return false;
}

SPIHost::~SPIHost() { close(fd_); }

bool SPIHost::Connect(const char *device, const Options &set_options) {
    if (fd_ >= 0)
        close(fd_);  // Was already open.
    if ((fd_ = open(device, O_RDWR)) < 0)
        return io_problem(device);

    options_ = set_options;
    // Right now, we only read the mode, but later should allow to set
    // it via options.
    int ret = ioctl(fd_, SPI_IOC_RD_MODE32, &options_.mode);
    if (ret < 0) return io_problem("Couldn't read mode");

    ret = ioctl(fd_, SPI_IOC_WR_BITS_PER_WORD, &options_.bits_per_word);
    if (ret < 0) return io_problem("Setting bits per word");

    // Let's read back to see that we set what we thought.
    ret = ioctl(fd_, SPI_IOC_RD_BITS_PER_WORD, &options_.bits_per_word);
    if (ret < 0) return io_problem("Couldn't read bits per word");
    if (options_.bits_per_word != set_options.bits_per_word)
        return io_problem("Bits per word didn't stick");

    ret = ioctl(fd_, SPI_IOC_WR_MAX_SPEED_HZ, &options_.speed_hz);
    if (ret < 0) return io_problem("Setting speed");

    ret = ioctl(fd_, SPI_IOC_RD_MAX_SPEED_HZ, &options_.speed_hz);
    if (ret < 0) return io_problem("Reading speed");
    if (options_.speed_hz != set_options.speed_hz) {
        fprintf(stderr, "Wanted speed: %.3fkHz, actual speed: %.3fkHz\n",
                set_options.speed_hz/1000.0, options_.speed_hz/1000.0);
    }
    return true;
}


bool SPIHost::TransferBuffer(const void *send, void *receive, size_t len,
                             bool is_last_in_transaction) {
    struct spi_ioc_transfer tr = {};
    tr.tx_buf = (unsigned long)send;
    tr.rx_buf = (unsigned long)receive;
    tr.len = (uint32_t) len;
    tr.speed_hz = options_.speed_hz;
    tr.delay_usecs = 0;
    tr.bits_per_word = options_.bits_per_word;
    tr.cs_change = is_last_in_transaction ? 1 : 0;

    if (options_.mode & SPI_TX_QUAD)
        tr.tx_nbits = 4;
    else if (options_.mode & SPI_TX_DUAL)
        tr.tx_nbits = 2;
    if (options_.mode & SPI_RX_QUAD)
        tr.rx_nbits = 4;
    else if (options_.mode & SPI_RX_DUAL)
        tr.rx_nbits = 2;
    if (!(options_.mode & SPI_LOOP)) {
        if (options_.mode & (SPI_TX_QUAD | SPI_TX_DUAL))
            tr.rx_buf = 0;
        else if (options_.mode & (SPI_RX_QUAD | SPI_RX_DUAL))
            tr.tx_buf = 0;
    }

    int ret = ioctl(fd_, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 0) return io_problem("Sending SPI message");
    return true;
}
