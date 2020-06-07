
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

  localparam integer WORD_BITS_SIZE = $clog2(WORD_BITS);
  reg [WORD_BITS-1:0] data_received;  // Register of size WORD_BITS + 1.
  reg [WORD_BITS_SIZE:0] counter;  // Count how many bits we received.

  reg [2:0] sck_buffer = 2'b00;
  wire rising = (sck_buffer[2:1] == 2'b01);  // now we can detect SCK rising edges

  //assign word_ready = rising;
  assign word_ready = counter[WORD_BITS_SIZE];

  // Assign the last WORD_BITS of data_received to the output.
  assign data_word_received = data_received;

  // Initialize the registers
  initial begin
    counter = {(WORD_BITS_SIZE + 1) {1'b0}};
    data_received = {(WORD_BITS) {1'b0}};
  end  // initial begin

  // Shift register.
  always @(posedge clk) begin
    if (rising) begin
      // Shift data_word received by one bit and include the new bit.
      data_received <= {data_received[WORD_BITS:0], in_bit};

      // Increment the counter
      counter <= counter + 1;

    end  // if (rising)
    // Update state
    sck_buffer <= {sck_buffer[1:0], sck};

    if (word_ready == 1'b1) counter[WORD_BITS_SIZE] <= 1'b0;

  end  // always@ (posedge clk and posedge sck)

endmodule  // SpiSecondary
