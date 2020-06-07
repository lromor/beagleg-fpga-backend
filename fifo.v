// A synchronous fifo that receives n bytes at a time and fills a fifo.
// Whenever RECORD_WORDS inputs are fed, a new record can be dequed.
// RECORD_WORDS and SLOTS need to be a power of 2.
// To feed an input or read a record, write_en/read_en must be set to high.
// In the next rising clock edge the fifo will be updated either by copying
// the new input or dequeing out from the fifo first record.
module Fifo #(
    parameter integer WORD_SIZE = 8,
    parameter integer RECORD_WORDS = 16,  // Number of inputs required to make a record.
    parameter integer SLOTS = 8  // Number of records, should be a power of 2
) (
   input logic clk, // Input clock
   input logic write_en, // Write data in on rising
   input wire [WORD_SIZE-1:0] data_in, // Input do the fifo
   input logic read_en, // Read on rising
   output logic full, // Rises when fifo is full
   output logic empty, // Rises when fifo is empty
   output [RECORD_WORDS*WORD_SIZE-1:0] data_out // Output data when reading
);
  localparam integer RECORD_SIZE_BITS = WORD_SIZE * RECORD_WORDS;
  localparam integer STORAGE_SIZE = SLOTS * RECORD_WORDS;
  localparam integer STORAGE_POS_SIZE = $clog2(STORAGE_SIZE);
  localparam integer RECORD_POS_SIZE = $clog2(RECORD_WORDS);

  reg [WORD_SIZE-1:0] storage[STORAGE_SIZE-1:0];

  // One bit more for the position for the size representation.
  reg [STORAGE_POS_SIZE:0] write_pos_r = 0;
  reg [STORAGE_POS_SIZE:0] read_pos_r = 0;

  wire [STORAGE_POS_SIZE:0] size = write_pos_r - read_pos_r;

  // The wires of the read/write positions are a bit less because we use
  // it as indexes for the storage and they need the proper modular arithmetic.
  wire [STORAGE_SIZE-1:0] write_pos_idx_w = write_pos_r[STORAGE_POS_SIZE - 1:0];
  wire [STORAGE_SIZE-1:0] read_pos_idx_w = read_pos_r[STORAGE_POS_SIZE - 1:0];

  wire empty = (size >> RECORD_POS_SIZE) == 0;
  wire full = (size == STORAGE_SIZE);
  wire do_write_w = write_en && !full;
  wire do_read_w = read_en && !empty;

  // Connect the output to the storage reading position.
  // Maybe a bit heavy on the final design. Use registers?
  // ¯\_(ツ)_/¯
  generate
    genvar i;
    for (i = 0; i < RECORD_WORDS; i = i + 1)
      assign data_out[(i + 1) * WORD_SIZE - 1:i * WORD_SIZE] = storage[read_pos_idx_w + i];
  endgenerate

  // Write stuff
  always @(posedge clk) begin
    if (do_write_w) begin
      // Increment the write position
      write_pos_r <= write_pos_r + 1;
      // Update the storage with the input.
      storage[write_pos_idx_w] <= data_in;
    end
    if (do_read_w) begin
      // Step the read pos.
      // Since our record is made of multiple inputs
      // we step of RECORD_WORDS.
      read_pos_r <= read_pos_r + RECORD_WORDS;
    end
  end
endmodule
