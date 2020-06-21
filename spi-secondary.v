
module SpiSecondary #(
    parameter integer WORD_BITS = 8,
    localparam integer WORD_BITS_SIZE = $clog2 (WORD_BITS)
) (
    input logic clk,

    // External interface
    input  logic sck,  // clock
    input  logic in_bit,  // main out secondary in bit
    output logic out_bit,  // main in secondary out bit
    input  logic cs,  // channel select

    // Bus interface
    output logic word_ready,  // New word received and setting the output with to_send
    output wire [WORD_BITS-1:0] data_word_received,  // data just received
    input wire [WORD_BITS-1:0] data_word_to_send  // data to send in next word
);
  reg [WORD_BITS-1:0] data;  // Register of size WORD_BITS + 1.

  // Count how many bits we received.
  // One bit for the overflow to see when byte is full.
  reg [WORD_BITS_SIZE:0] counter = {(WORD_BITS_SIZE + 1) {1'b0}};

  reg [2:0] sck_buffer = 2'b00;
  wire rising = (sck_buffer[2:1] == 2'b01);  // now we can detect SCK rising edges
  wire falling = (sck_buffer[2:1] == 2'b10);

  assign word_ready = counter[WORD_BITS_SIZE];

  // Assign the last WORD_BITS of data_received to the output.
  assign data_word_received = data;

  // Shift register.
  always @(posedge clk) begin
    if (cs) begin
      data <= data_word_to_send;

      // Last bit!
      out_bit <= data_word_to_send[WORD_BITS - 1];
    end

    if (rising) begin
      // Shift data_word received by one bit and include the new bit.
      data <= {data[WORD_BITS - 2:0], in_bit};
    end  // if (rising)

    if (falling) begin
      // Set msb to output
      out_bit <= data[WORD_BITS - 1];

      // Increment the counter
      counter <= counter + 1;
    end

    // Update state
    sck_buffer <= {sck_buffer[1:0], sck};

    // Overflow! We stored a full word
    if (word_ready == 1'b1) begin
      // Reset the counter
      counter[WORD_BITS_SIZE] <= 1'b0;
      // Update the data with the new word to be sent
      data <= data_word_to_send;
    end

  end  // always@ (posedge clk and posedge sck)
endmodule  // SpiSecondary
