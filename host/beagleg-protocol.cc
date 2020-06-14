
#include "spi.h"

#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

// The protocol elements. Right now just pretend
namespace beagleg {
struct MotionSegment {
    uint32_t count_steps;    // see beagleg/src/motion-queue.h for real deal.
};

struct QueueStatus {
    uint32_t counter : 24; // remaining number of cycles to be performed
    uint32_t index : 8;    // represent the executing slot [0 to QUEUE_LEN - 1]
};
}  // namespace beagleg


// These numbers need to match with constants in the FPGA impl.

// How many bytes there are in one record.
static constexpr int BYTES_PER_FIFO_RECORD = sizeof(beagleg::MotionSegment);

// Number of total slots in the fifo. We need this to determine
// how many we can read.
static constexpr int FIFO_SLOTS = 16;

// This needs to be in-sync with FPGA impl.
enum Command {
    CMD_NO_OP = 0x00,  // Just send one byte to receive fifo free
    CMD_STATUS = 0x01, // Send 5 bytes, receive fifo free + status word
    CMD_WRITE = 0x02,
    CMD_READ = 0x03,
};

// Put terminal in canonical mode, read chars.
class TerminalInput {
public:
    TerminalInput() {
        if (tcgetattr(0, &orig_state_)){
            fprintf(stderr, "Error getting current terminal settings\n");
            return;  // shrug.
        }

        termios modified_tios = orig_state_;

        modified_tios.c_lflag &= ~ICANON;   // Get each character.
        modified_tios.c_lflag &= ~ECHO;     // Don't want to echo

        if (tcsetattr(0, TCSANOW, &modified_tios)) {
            printf("Uh, couldn't set terminal settings\n");
        }
    }

    ~TerminalInput() {
        // Reset to previous state.
        tcsetattr(0, TCSANOW, &orig_state_);
    }

    char read_char() {
        int c;
        read(STDIN_FILENO, &c, 1);
        return c;
    }

private:
    termios orig_state_;
};

class BeagleGSPIProtocol {
public:
    BeagleGSPIProtocol(SPIHost *channel) : spi_channel_(channel) {}

    // Get number of free slots.
    int GetFreeSlots() {
        const char cmd = CMD_NO_OP;
        char val;
        spi_channel_->TransferBuffer(&cmd, &val, 1);
        return val;
    }

    // Get queue status. Also return number of free slots.
    int GetQueueStatus(beagleg::QueueStatus *status) {
        const size_t tx_len = 1 + sizeof(beagleg::QueueStatus);
        char tx_buffer[tx_len];
        char rx_buffer[tx_len];
        tx_buffer[0] = CMD_STATUS;
        spi_channel_->TransferBuffer(tx_buffer, rx_buffer, tx_len);
        memcpy(status, rx_buffer + 1, sizeof(*status));
        return rx_buffer[0];
    }

private:
    SPIHost *const spi_channel_;
};

static void print_free_slots(int slots) {
    printf("Free slots: %d\n", slots);
}

static void print_status(const beagleg::QueueStatus &status) {
    printf("Status: counter:%d; index:%d ", status.counter, status.index);
    const char *raw = (const char *) &status;
    printf("[0x%02x 0x%02x 0x%02x 0x%02x]\n", raw[0], raw[1], raw[2], raw[3]);
}

int main(int argc, char *argv[]) {
    const char *device = "/dev/spidev0.0";
    int opt;

    while ((opt = getopt(argc, argv, "D:")) != -1) {
        switch (opt) {
        case 'D':
            device = strdup(optarg);
            break;
        }
    }

    SPIHost::Options options;
    options.bits_per_word = 8;
    options.speed_hz = 500'000;  // Let's be slow for now

    SPIHost spi;
    if (!spi.Connect(device, options)) {
        fprintf(stderr, "Couldn't connect to SPI bus %s\n", device);
        return 1;
    }

    printf("Using %s\n", device);
    printf("Assuming\n\tsizeof(MotionSegment) = %d\n\tsizeof(QueueStatus) = %d\n\n",
           (int)sizeof(beagleg::MotionSegment),
           (int)sizeof(beagleg::QueueStatus));

    printf("User interaction:\n"
           "\tf    - no-op. Just read fifo free slots\n"
           "\ts    - read status word (fifo free + status)\n"
           "\tESC  - quit\n"
        );

    beagleg::QueueStatus status = {};
    BeagleGSPIProtocol protocol(&spi);
    TerminalInput terminal;

    for (;;) {
        fprintf(stderr, "------[ f: free-slots, s: status; ESC: quit] ------------------\n");
        const char key = terminal.read_char();
        switch (key) {
        case 'f':
            print_free_slots(protocol.GetFreeSlots());
            break;

        case 's':  // Read status
            print_free_slots(protocol.GetQueueStatus(&status));
            print_status(status);
            break;

        case 'q':
        case 'Q':
        case 0x1b:  // Escape
            fprintf(stderr, "Signing off; Have a wonderful day!\n");
            return 0;

        default:
            fprintf(stderr, "? '%c' (0x%02x)\n", key, key);
        }
    }
}
