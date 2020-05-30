
// A fifo that receives one byte at a time and fills a fifo.
// Whenever RECORD_SIZE_BYTES is filled,
module Fifo #(
    parameter integer RECORD_SIZE_BYTES = 16,
    parameter integer SLOTS = 32
) (
  input logic 			       rst, input logic rst,

  // Input -- getting rcords one byte at a time.

  output logic 			       in_ready, // We're ready to receive a byte. Might
  // block until data is picked up other end.
  input wire [7:0] 		       in_byte, // byte to receive for the next record.
  input logic 			       in_clk, // Clock in new data.

  output logic 			       out_available, // There is a record available
  input logic 			       request_record, //
  output logic 			       out_record_ready, //
  // Is this a good idea to do, using up so many wires for a full record ?
  output reg [RECORD_SIZE_BYTES*8-1:0] out_record);

endmodule

module BeagleGFPGABackend (
  input clk
);
endmodule

module top ();
  BeagleGFPGABackend beagleg(
	 .clk(system_clock)
			      );

endmodule
