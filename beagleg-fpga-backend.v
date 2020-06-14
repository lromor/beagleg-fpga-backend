// Should this file be called 'top.v' ?

module top (input clk,
  output led_red,
  output led_green,
  output led_blue,
  input spi_mosi,
  input spi_sck,
  input spi_cs,
  output spi_miso,
  output p1,
  output p2,
  output p3,
  output p4,
  output p5,
  output p6,
  output p7,
  output p8
);

  localparam FIFO_WORD_SIZE 8;
  localparam FIFO_RECORD_WORDS 2;
  localparam FIFO_SLOTS 16;

  LedBlinker blinker(.clk(clk),
                     .led_red(led_red),
                     .led_green(led_green),
                     .led_blue(led_blue));

  // Spi
  reg [7:0] spi_secondary_data_r;
  wire [7:0] spi_main_data_w;
  wire spi_main_data_ready_w;
  wire spi_secondary_data_w = spi_secondary_data_r;


  // Fifo
  wire [$clog2(FIFO_SLOTS):0] fifo_size;
  wire fifo_full_w;
  wire fifo_empty_w;
  wire fifo_write_en;

  // FSM
  reg op = 1'b0;
  reg state = 0;

  assign spi_secondary_data_w = (op == 0) & fifo_size;
  assign fifo_write_en = (op == 1) & spi_main_data_ready_w;

  // Use the fifo as buffer for the data collected
  // from the spi.
  Fifo #(.WORD_SIZE(FIFO_WORD_SIZE),
         .RECORD_WORDS(FIFO_RECORD_WORDS),
         .SLOTS(FIFO_SLOTS)) fifo(.clk(clk),
                                  .size(fifo_size),
                                  // Write stuff
                                  .write_en(fifo_write_en),
                                  .data_in(spi_main_data_w),
                                  // Status
                                  .full(fifo_full_w),
                                  .empty(fifo_empty_w),
                                  // Read stuff
                                  .read_en(deplete_r),
                                  .data_out(fifo_out));

  SpiSecondary spi_secondary(.clk(clk),
                             .sck(spi_sck),
                             .in_bit(spi_mosi),
                             .out_bit(spi_miso),
                             .data_word_received(spi_main_data_w),
                             .data_word_to_send(spi_secondary_data_w),
                             .word_ready(spi_main_data_ready_w));

  end

endmodule
