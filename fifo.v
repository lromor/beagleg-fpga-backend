// A fifo that receives one byte at a time and fills a fifo.
// Whenever RECORD_SIZE_BYTES is filled,
module Fifo #(
    parameter integer INPUT_SIZE_BYTES = 1,
    parameter integer RECORD_SIZE = 16, // Number of inputs required to make a record.
    parameter integer SLOTS = 8 // Number of records, should be a power of 2
  ) (
   input logic                                        clk, // Input clock
   input logic                                        write_en, // Write data in on rising
   input wire [INPUT_SIZE_BYTES*8-1:0]                data_in, // Input do the fifo
   input logic                                        read_en, // Read on rising
   output logic                                       full, // Rises when fifo is full
   output logic                                       empty, // Rises when fifo is empty
   output [(RECORD_SIZE*INPUT_SIZE_BYTES)*8-1:0] data_out // Output data when reading
   );

   localparam integer                  INPUT_SIZE_BITS = INPUT_SIZE_BYTES * 8;
   localparam integer                  RECORD_SIZE_BITS = INPUT_SIZE_BITS * RECORD_SIZE;
   localparam integer                  STORAGE_SIZE = SLOTS * RECORD_SIZE;
   localparam integer                  STORAGE_POS_SIZE = $clog2(STORAGE_SIZE);
   localparam integer                  RECORD_POS_SIZE = $clog2(RECORD_SIZE);

   reg [INPUT_SIZE_BITS-1:0]          storage[STORAGE_SIZE-1:0];
   reg [STORAGE_POS_SIZE:0]            write_pos_r;
   reg [STORAGE_POS_SIZE:0]            read_pos_r;

   wire [STORAGE_POS_SIZE:0]           size = write_pos_r - read_pos_r;
   wire [STORAGE_SIZE-1:0]             write_pos_idx_w = write_pos_r[STORAGE_POS_SIZE-1:0];
   wire [STORAGE_SIZE-1:0]             read_pos_idx_w = read_pos_r[STORAGE_POS_SIZE-1:0];

   wire empty = (size >> RECORD_POS_SIZE) == 0;
   wire full = (size == STORAGE_SIZE);
   wire do_write_w = write_en && !full;
   wire do_read_w = read_en && !empty;

   initial write_pos_r = 0;
   initial read_pos_r = 0;

   // Connect the output to the storage reading position.
   // Maybe a bit heavy on the final design. Use registers?
   // ¯\_(ツ)_/¯
   generate
      genvar i;
      for (i = 0; i < RECORD_SIZE; i = i + 1)
        assign data_out[(i+1)*INPUT_SIZE_BITS-1:i*INPUT_SIZE_BITS] = storage[read_pos_idx_w + i];
   endgenerate

   // Write stuff
   always@(posedge clk)
     begin
        if (do_write_w)
          begin
             // Increment the write position
             write_pos_r <= write_pos_r + 1;
             // Update the storage with the input.
             storage[write_pos_idx_w] <= data_in;
          end
        if (do_read_w)
          begin
             // Step the read pos.
             // Since our record is made of multiple inputs
             // we step of RECORD_SIZE.
             read_pos_r <= read_pos_r + RECORD_SIZE;
          end
     end

endmodule
