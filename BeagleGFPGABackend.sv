// -- mostly sketching out right now.

// A fifo that receives one byte at a time and fills a fifo.
// The output is quantized by 'record size'.
module Fifo #(
     parameter integer RECORD_SIZE_BYTES = 16,
     parameter integer SLOTS = 32,
)(
  input logic clk, input logic rst,

  // Put -- inserting records one byte at a time.
  output logic 	   in_ready,
  input wire [7:0] in_byte,           // byte to receive for the next record.
  input logic 	   in_clk,            // Clock in new data.

  // Get,
  output logic 	   out_available,     // There is a record available
  input logic 	   request_record,    //
  output logic 	   out_record_ready,  //

  // Is this a good idea to do, using up so many wires for a full record ?
  output reg [RECORD_SIZE_BYTES*8-1:0] out_record
  );
/*... */
endmodule  // Fifo

module SpiSecondary #(
      parameter integer WORD_BITS = 8,
)(
  input logic clk, input logic rst,

  // External interface
  input  logic neg_enable,    // ~Enable: only then we receive
  input  logic sck,           // clock
  input  logic in_bit,        // main out secondary in bit
  output logic out_bit,       // main in secondary out bit

  // Bus interface
  output logic word_ready,    // Ready to pick up received and set
  output reg [WORD_BITS-1:0] data_word_received,  // data just received
  input wire [WORD_BITS-1:0] data_word_to_send,  // data to send in next word
 );
endmodule // SpiSecondary

module StepEngine #(
  parameter integer MOTORS = 8,
  parameter integer MAX_MOVE_STEP_COUNT = 65535,

  // We need double the resolution.
  parameter integer MOTOR_REGISTER_WIDTH = 2 * $clog2(MAX_MOVE_STEP_COUNT),
)(
  input logic clk, input logic rst,

  output wire [MOTORS-1:0] step,
  output wire [MOTORS-1:0] direction,

  // getting the input record. That should be a typedef similar
  // https://github.com/hzeller/beagleg/blob/master/src/motion-queue.h#L50
  );

endmodule // StepEngine

module top #(
)(
  // External interface
  // Spi bits
  // motor outputs
  );

endmodule
