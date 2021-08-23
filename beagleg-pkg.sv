// particular shared with the host side.
package beagleg_pkg;
  // TODO: make fixed point value a union maybe ?

  typedef logic [31:0] uint32_t;

  // struct is reversed to c-struct. TODO: use streaming operator to decode ?
  typedef struct packed {
    uint32_t dummy1, dummy2;
    uint32_t jerk;
    uint32_t target_accel;
    uint32_t current_accel;
    uint32_t target_speed;
    uint32_t current_speed;  // speed is delta steps per sample
    uint32_t target_steps;  // target step count.
  } motion_segment_t;

  // Yosys not supported yet: $bits() yet
  parameter integer MotionSegmentBits = 8 * 32;  // $bits(motion_segment_t);

  parameter integer FifoDepth = 8;

  // Must match enum in beagleg-protocol.cc
  typedef enum {
    CMD_NO_OP      = 0,  // Get fifo free slots
    CMD_STATUS     = 1,  // Get status word
    CMD_WRITE_FIFO = 2   // Send segments to fifo
  } command_e;
endpackage
