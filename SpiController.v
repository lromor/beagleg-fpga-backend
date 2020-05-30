
module SpiSecondary #(
    parameter integer WORD_BITS = 8
) (
  input logic clk, input reg rst,

  // External interface
  input  logic neg_enable,    // ~Enable: only then we receive
  input  logic sck,           // clock
  input  logic in_bit,        // main out secondary in bit
  output logic out_bit,       // main in secondary out bit

  // Bus interface
  output logic word_ready,    // Ready to pick up received and set
  output wire [WORD_BITS-1:0] data_word_received,  // data just received
  input wire [WORD_BITS-1:0] data_word_to_send  // data to send in next word
 );

  localparam integer WORD_BITS_SIZE = clog2(WORD_BITS);
  reg [WORD_BITS:0] data_received; // Register of size WORD_BITS + 1.
  reg [WORD_BITS_SIZE:0] counter; // Count how many bits we received.

  assign word_ready = counter[WORD_BITS_SIZE];

  // Assign the last WORD_BITS of data_received to the output.
  genvar i;
  generate
    for (i = 0; i < WORD_BITS; i++)
      assign data_word_received[i] = data_received[i + 1];
  endgenerate


  // Shift register.
  always@((posedge clk and posedge sck) or posedge rst) begin
    if (rst)
      begin
        counter <= {(WORD_BITS_SIZE + 1){1'b0}};
        data_received <= {(WORD_BITS + 1){1'b0}};
      end // if (rst)
    else
      begin
        // Shift data_word received by one bit.
        data_received <= data_received << 1;

        // Include the new bit.
        data_received[0] <= in_bit;

        // Increment the counter
        counter <= counter + 1;

        if (word_ready == 1)
          word_ready <= 0;

      end // else: !if(rst)
  end // always@ (posedge clk and posedge sck)

  // Count the number of ones.
  // For instance: 8 = 1000, 8 - 1 = 0111.
  // Until 0111 becomes zero, shift right
  // and increment the output.
  function [31:0] clog2;
    input [31:0] value;
    reg [31:0] num;
    begin
      num = (value - 1);
      for (clog2 = 0; (num > 0); clog2 = (clog2 + 1))
        num = (num >> 1);
    end
  endfunction
endmodule  // SpiSecondary
