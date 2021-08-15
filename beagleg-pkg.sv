// particular shared with the host side.
package beagleg_pkg;
  // TODO: make fixed point value a union maybe ?

  // First step: distance per sample, i.e. constant speed.
  typedef struct packed {
    logic [31:0] sample_count;     // how many samples we run
    logic [31:0] delta_distance_per_sample;  // as 16.16 unsigned fixed point
  } motion_segment_t;

  // Yosys not supported yet: $bits() yet
  parameter integer MotionSegmentBits = 64;  // $bits(motion_segment_t);

  // Must match enum in beagleg-protocol.cc
  typedef enum {
    CMD_NO_OP      = 0,  // Get fifo free slots
    CMD_STATUS     = 1,  // Get status word
    CMD_WRITE_FIFO = 2   // Send segments to fifo
  } command_e;
endpackage
