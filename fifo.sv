// A synchronous fifo that receives n bytes at a time and fills a fifo.
// Whenever RecordWords inputs are fed, a new record can be dequed.
// RecordWords and Depth need to be a power of 2.
// To feed an input or read a record, write_en/read_en must be set to high.
// In the next rising clock edge the fifo will be updated either by copying
// the new input or dequeing out from the fifo first record.
//
// TODO: write_en and read_en sounds like the wrong word. read_req ?
module fifo #(
    parameter integer WordSize = 8,
    parameter integer RecordWords = 16,  // Number of inputs required to make a record.
    parameter integer Depth = 8,  // Number of records, should be a power of 2

    // Derived
    localparam integer RecordSizeBits = WordSize * RecordWords,
    localparam integer StorageSize = Depth * RecordWords,
    localparam integer StoragePosSize = $clog2(StorageSize),
    localparam integer RecordPosSize = $clog2(RecordWords)
) (
    input logic clk,  // Input clock

    input logic                write_en,  // Write data in on rising
    input logic [WordSize-1:0] data_in,   // Input to the fifo

    input  logic                      read_en,  // Read on rising
    output logic [RecordSizeBits-1:0] data_out, // Output data when reading

    output logic                    full,   // Rises when fifo is full
    output logic                    empty,  // Rises when fifo is empty
    output logic [StoragePosSize:0] size    // Size of fifo in words
);

  logic [WordSize-1:0] storage[StorageSize];

  // One bit more for the position for the size representation.
  logic [StoragePosSize:0] write_pos_r = 0;
  logic [StoragePosSize:0] read_pos_r = 0;

  assign size = write_pos_r - read_pos_r;

  // The wires of the read/write positions are a bit less because we use
  // it as indexes for the storage and they need the proper modular arithmetic.
  wire [StorageSize-1:0] write_pos_idx_w = write_pos_r[StoragePosSize-1:0];
  wire [StorageSize-1:0] read_pos_idx_w = read_pos_r[StoragePosSize-1:0];

  assign empty = (size >> RecordPosSize) == 0;
  assign full  = (size == StorageSize);
  wire do_write_w = write_en && !full;
  wire do_read_w = read_en && !empty;

  // Connect the output to the storage reading position.
  // Maybe a bit heavy on the final design. Use registers?
  // ¯\_(ツ)_/¯
  generate
    genvar i;
    for (i = 0; i < RecordWords; i = i + 1)
      assign data_out[(i+1)*WordSize-1:i*WordSize] = storage[read_pos_idx_w+i];
  endgenerate

  // Write stuff
  always @(posedge clk) begin
    if (do_write_w) begin
      // We read one byte at a time; increment by that.
      write_pos_r <= write_pos_r + 1;
      // Update the storage with the input.
      storage[write_pos_idx_w] <= data_in;
    end
    if (do_read_w) begin
      // Step the read pos.
      // Since our record is made of multiple inputs
      // we step of RecordWords.
      read_pos_r <= read_pos_r + RecordWords;
    end
  end  // always @ (posedge clk)

`ifdef FORMAL
  always_comb begin
    assert (size <= StorageSize);
    //assert(write_en ? size : 1 == $past(write_en ? size - 1 : 1));
  end

`endif  // FORMAL
endmodule
