// A fifo that receives one byte at a time and fills a fifo.
// Whenever RECORD_SIZE_BYTES is filled,
module Fifo #(
    parameter integer INPUT_SIZE_BYTES = 1,
    parameter integer RECORD_SIZE = 1, // Number of inputs required to make a record.
    parameter integer SLOTS = 4 // Number of records
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

   reg [STORAGE_POS_SIZE-1:0]            write_pos_r;
   reg [STORAGE_POS_SIZE-1:0]            read_pos_r;

   wire do_write_w = write_en & ~full;
   wire do_read_w = read_en & ~empty;

   wire [STORAGE_POS_SIZE-1:0] size;
   assign size = write_pos_r - read_pos_r;

   wire empty = (size >> RECORD_POS_SIZE) == 0;
   wire full = size == (STORAGE_SIZE - 1);

   initial write_pos_r = {(STORAGE_POS_SIZE){1'b0}};
   initial read_pos_r = {(STORAGE_POS_SIZE){1'b0}};

   integer i;
  
   // Write stuff
   always@(posedge clk)
     begin
        if (do_write_w)
          begin
             write_pos_r <= write_pos_r + 1;
             storage[write_pos_r] <= data_in;
          end
        if (do_read_w)
          begin
             read_pos_r <= read_pos_r + RECORD_SIZE;
             for (i = 0; i <= RECORD_SIZE; i = i + 1)
               data_out[(i+1)*INPUT_SIZE_BITS-1:i*INPUT_SIZE_BITS] <= storage[read_pos_r+i];
          end
     end

endmodule
