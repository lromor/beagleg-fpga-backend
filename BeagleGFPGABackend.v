// Should this file be called 'top.v' ?

module top (input clk,
  output led_red,
  output led_green,
  output led_blue,
  input mosi,
  input sck,
  input cs,
  output miso,
  output p1,
  output p2,
  output p3,
  output p4,
  output p5,
  output p6,
  output p7,
  output p8,
  output p9);

  LedBlinker blinker(.clk(clk),
                      .led_red(led_red),
                      .led_green(led_green),
                      .led_blue(led_blue));

  // Spi
  wire [7:0] spi_main_data_w;
  wire spi_main_data_ready_w;

  // Fifo
  wire fifo_full_w;
  wire fifo_empty_w;
  wire [7:0] data_out = {p1, p2, p3, p4, p5, p6, p7, p8};

  // Depleting test
  reg deplete_r = 1'b0;
  reg [15:0] fifo_out;

  // Use the fifo as buffer for the data collected
  // from the spi.
  Fifo #(.INPUT_SIZE_BYTES(1),
         .RECORD_SIZE(2),
         .SLOTS(16)) fifo(.clk(clk),
                          // Write stuff
                          .write_en(spi_main_data_ready_w),
                          .data_in(spi_main_data_w),
                          // Status
                          .full(fifo_full_w),
                          .empty(fifo_empty_w),
                          // Read stuff
                          .read_en(deplete_r),
                          .data_out(fifo_out));

  SpiSecondary spi_secondary(.clk(clk),
                              .sck(sck),
                              .in_bit(mosi),
                              .data_word_received(spi_main_data_w),
                              .word_ready(spi_main_data_ready_w));

  // Whenever it's full, deplete it in data_out.
  always @(posedge clk) begin
    if (fifo_full_w) deplete_r <= 1'b1;

    if (deplete_r) begin
      // For testing purposes
      data_out <= fifo_out[7:0];
      if (fifo_empty_w) deplete_r <= 0;
    end
  end

endmodule
