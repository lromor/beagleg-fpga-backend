
#include "spi.h"

#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <inttypes.h>

#include <algorithm>

#if USE_SIMULATION
#  include "../sim/hsg-sim.h"
#endif

// The protocol elements.
//   beagleg/src/motion-queue.h will be the real deal.
// For now just keeping in sync with
//   ../beagleg-pkg.sv  motion_segment_t
namespace beagleg {
struct MotionSegment {
  uint32_t target_steps;     // Total steps to be generated
  uint32_t current_speed;
  uint32_t target_speed;
  uint32_t current_accel;    // If we don't have jerk, needs to be target_accel
  //uint32_t target_accel;
  //uint32_t jerk;
};

struct QueueStatus {
  uint32_t counter : 24; // remaining number of cycles to be performed
  uint32_t index : 8;    // represent the executing slot [0 to QUEUE_LEN - 1]
};
}  // namespace beagleg


static uint32_t doubleToIntFraction(double v) {
  assert(v < 1.0);
  uint64_t uint_val = 0x1'00'00'00'00;
  uint_val *= v;
  return uint_val;
}

// These numbers need to match with constants in the FPGA impl.

// How many bytes there are in one record.
static constexpr int BYTES_PER_FIFO_RECORD = sizeof(beagleg::MotionSegment);

// This needs to be in-sync with FPGA impl.
enum Command {
  CMD_NO_OP      = 0x00,  // Just send one byte to receive fifo free
  CMD_STATUS     = 0x01, // Send 5 bytes, receive fifo free + status word
  CMD_WRITE_FIFO = 0x02,
  CMD_READ_FIFO  = 0x03,
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
    char tx_buffer[tx_len] = {};
    char rx_buffer[tx_len];
    tx_buffer[0] = CMD_STATUS;
    spi_channel_->TransferBuffer(tx_buffer, rx_buffer, tx_len);
    memcpy(status, rx_buffer + 1, sizeof(*status));
    return rx_buffer[0];
  }

  // Attempts to send motion segments. Only sends amount possible.
  // Returns number of segments sent.
  int SendMotionSegments(const beagleg::MotionSegment *segments, int count) {
    fprintf(stderr, "Writing first byte to read free slots, but keep CS low to continue transaction...\n");
    const char command = CMD_WRITE_FIFO;
    uint8_t free_slots;
    if (!spi_channel_->TransferBuffer(&command, &free_slots, 1, false))
      return -1;

    if (free_slots < count) {
      fprintf(stderr, "Available fifo space of %d < requested %d\n",
              free_slots, count);
    }

    const int segment_tx_count = std::min((int)free_slots, count);
    const int segment_byte_len = segment_tx_count * sizeof(beagleg::MotionSegment);
    char rx_buffer[segment_byte_len];   // Let's see what it sends back
    fprintf(stderr, "Sending actual data; %d elements = %d bytes\n",
            segment_tx_count, segment_byte_len);
    if (!spi_channel_->TransferBuffer(segments, rx_buffer, segment_byte_len))
      return -1;
    return segment_tx_count;
  }

private:
  SPIHost *const spi_channel_;
};


static void print_free_slots(int slots) {
  printf("Free slots: %d\n", slots);
}

static void print_status(const beagleg::QueueStatus &status) {
  printf("Status: counter:%d; index:%d\n", status.counter, status.index);
}

static void ReadNumber(TerminalInput *terminal, const char *prompt,
                       uint32_t *out, uint32_t *prefix_zero = nullptr) {
  printf("%s", prompt);
  fflush(stdout);
  char k;

  // For now, just integer
  *out = 0;
  if (prefix_zero) *prefix_zero = 0;
  bool only_zero_so_far = true;
  while ((k = terminal->read_char()) != '\n') {
    if (k >= '0' && k <= '9') {
      write(STDOUT_FILENO, &k, 1);
      *out = *out * 10 + (k - '0');
      only_zero_so_far &= (k == '0');
      if (prefix_zero && only_zero_so_far) ++*prefix_zero;
    } else {
      k = '\007';  // beep.
      write(STDOUT_FILENO, &k, 1);
    }
  }
  printf(" \u2713\n");  // checkmark.
}

static void ReadFraction(TerminalInput *terminal, const char *prompt,
                         double *out) {
  uint32_t digits, prefix_zero;
  ReadNumber(terminal, prompt, &digits, &prefix_zero);
  char reassembled[32];
  if (prefix_zero > 0) {
    snprintf(reassembled, sizeof(reassembled), "0.%0*d%u",
             prefix_zero, 0, digits);
  } else {
    snprintf(reassembled, sizeof(reassembled), "0.%u\n", digits);
  }
  *out = strtod(reassembled, nullptr);
}

static void ReadSegment(TerminalInput *terminal, beagleg::MotionSegment *segment) {

  ReadNumber(terminal, "Output steps to move: ", &segment->target_steps);
  double c_speed, t_speed;
  ReadFraction(terminal, "Current Speed (steps per sample): 0.", &c_speed);
  segment->current_speed = doubleToIntFraction(c_speed);
  ReadFraction(terminal, "Target Speed                    : 0.", &t_speed);
  segment->target_speed = doubleToIntFraction(t_speed);

  if (segment->current_speed != segment->target_speed) {
    double c_accel;
    ReadFraction(terminal, "Acceleration (steps per sample²): 0.", &c_accel);
    // For now, we don't use jerk, so acceleration is a constant.
    /*segment->target_accel = */segment->current_accel
      = doubleToIntFraction(c_accel);
    fprintf(stderr, "Taking %.1f samples to reach speed\n",
            (t_speed - c_speed) / c_accel);
  }
}

static void SendSegment(const beagleg::MotionSegment &segment, BeagleGSPIProtocol *protocol) {
  const int segments_written = protocol->SendMotionSegments(&segment, 1);
  if (segments_written) {
    printf("Wrote segment.\n");
  } else {
    printf("FIFO full. Didn't write segment\n");
  }
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
  options.verbose = true;

#if USE_SIMULATION
  SPIHost spi(StepGeneratorModuleSim::Init(argc, argv));
#else
  SPIHost spi;
#endif

  if (!spi.Connect(device, options)) {
    fprintf(stderr, "Couldn't connect to SPI bus %s (change with -D)\n", device);
    return 1;
  }

  printf("Using %s (change with -D)\n", device);
  printf("Assuming\n\tsizeof(MotionSegment) = %d\n\tsizeof(QueueStatus) = %d\n\n",
         (int)sizeof(beagleg::MotionSegment),
         (int)sizeof(beagleg::QueueStatus));

  printf("User interaction:\n"
         "\tf    - no-op. Just read fifo free slots\n"
         "\ts    - read status word (fifo free + status)\n"
         "\tw    - read step count and send motion segment to fifo\n"
         "\tW    - send motion segment to fifo with previously with lower-case 'w' read steps.\n"
         "\tESC  - quit\n"
         );

  static constexpr char nothing_to_do[] = "¯\\_(ツ)_/¯";

  beagleg::MotionSegment segment = {};
  segment.target_steps = 100;
  segment.current_speed = doubleToIntFraction(0.0);
  segment.target_speed = doubleToIntFraction(0.2);
  segment.current_accel = doubleToIntFraction(0.0005);
  beagleg::QueueStatus status;
  BeagleGSPIProtocol protocol(&spi);
  TerminalInput terminal;

  for (;;) {
    fprintf(stderr, "------[ f: free-slots, s: status; w: write; W: write same again; ESC: quit] --------------\n");
    const char key = terminal.read_char();
    switch (key) {
    case 'f':
    case ' ':   // Also space key is a wonderful way to send no-op.
      print_free_slots(protocol.GetFreeSlots());
      break;

    case 's':  // Read status
      print_free_slots(protocol.GetQueueStatus(&status));
      print_status(status);
      break;

    case 'w':
      ReadSegment(&terminal, &segment);
      SendSegment(segment, &protocol);
      break;

    case 'W':
      // Send previously read segment.
      fprintf(stderr, "Sending previous segment\n");
      SendSegment(segment, &protocol);
      break;

    case 'q':
    case 'Q':
    case 0x04:  // Ctrl-D
    case 0x1b:  // Escape
      fprintf(stderr, "Signing off; Have a wonderful day!\n");
      return 0;

    default:
      fprintf(stderr, "input key \e[1;31m0x%02X\e[0m: \e[1;33m%s\e[0m\n", key, nothing_to_do);
    }
  }
}
