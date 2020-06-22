// Definitions relevant for the implementation, and in
// particular shared with the host side.
package beagleg;
  // Yosys not supported yet: $bits() yet, so we can't just
  // define the type then determine its size.
  // So we do it the other way around :)
  parameter integer MotionSegmentBits = 32;

  // Yosys does not support structs yet, so this will be
  // interesting. For now, we only have a very simple data
  // type, so no big deal yet.
  typedef logic [MotionSegmentBits-1:0] MotionSegment;

  // Must match enum in beagleg-protocol.cc
  // (comments written like that because
  // https://github.com/google/verible/issues/336 )
  typedef enum {
    // Get fifo free slots
    CMD_NO_OP = 0,

    // Get status word
    CMD_STATUS = 1,

    // Send segments to fifo
    CMD_WRITE_FIFO = 2
  } command_e;
endpackage
