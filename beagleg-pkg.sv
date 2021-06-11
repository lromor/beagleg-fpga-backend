// particular shared with the host side.
package beagleg_pkg;
  // Yosys not supported yet: $bits() yet, so we can't just
  // define the type then determine its size.
  // So we do it the other way around :)
  parameter integer MotionSegmentBits = 32;

  // Yosys does not support structs yet, so this will be
  // interesting. For now, we only have a very simple data
  // type, so no big deal yet.
  typedef logic [MotionSegmentBits-1:0] MotionSegment;

  // Must match enum in beagleg-protocol.cc
  typedef enum {
    CMD_NO_OP      = 0,  // Get fifo free slots
    CMD_STATUS     = 1,  // Get status word
    CMD_WRITE_FIFO = 2   // Send segments to fifo
  } command_e;
endpackage
