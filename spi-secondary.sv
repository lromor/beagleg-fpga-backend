//
// A SPI client side with configurable word size
//
module spi_secondary #(
    parameter integer WordBits = 8,

    // Derived
    localparam integer WordBitSize = $clog2(WordBits)
) (
    input logic clk,

    // External interface
    input  logic sck,  // clock
    input  logic in_bit,  // main out secondary in bit
    output logic out_bit,  // main in secondary out bit
    input  logic cs,  // channel select

    // Bus interface
    output logic word_ready,  // New word received and setting the output with to_send
    output logic [WordBits-1:0] data_word_received,  // data just received
    input logic [WordBits-1:0] data_word_to_send  // data to send in next word
);
  logic [WordBits-1:0] data = 0;  // Register of size WordBits + 1.

  // Count how many bits we received.
  // One bit for the overflow to see when byte is full.
  logic [WordBitSize:0] counter = 0;


  logic [2:0] sck_buffer = 2'b00;
  wire rising = (sck_buffer[2:1] == 2'b01);  // now we can detect SCK rising edges
  wire falling = (sck_buffer[2:1] == 2'b10);

  assign word_ready = counter[WordBitSize];

  // Assign the last WordBits of data_received to the output.
  assign data_word_received = data;

  // Shift register.
  always_ff @(posedge clk) begin
    if (cs) begin
      data <= data_word_to_send;

      // Last bit!
      out_bit <= data_word_to_send[WordBits-1];
    end

    if (rising) begin
      // Shift data_word received by one bit and include the new bit.
      data <= {data[WordBits-2:0], in_bit};
    end  // if (rising)

    if (falling) begin
      // Set msb to output
      out_bit <= data[WordBits-1];

      // Increment the counter
      counter <= counter + 1;
    end

    // Update state
    sck_buffer <= {sck_buffer[1:0], sck};

    // Overflow! We stored a full word
    if (word_ready == 1'b1) begin
      // Reset the counter
      counter[WordBitSize] <= 1'b0;
      // Update the data with the new word to be sent
      data <= data_word_to_send;
    end

  end  // always@ (posedge clk and posedge sck)
endmodule  // SpiSecondary
