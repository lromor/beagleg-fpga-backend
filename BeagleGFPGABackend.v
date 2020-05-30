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

  wire [7:0] data_out = {p1, p2, p3, p4, p5, p6, p7, p8};

  SpiSecondary spi_secondary(.clk(clk),
    .sck(sck),
    .in_bit(mosi),
    .data_word_received(data_out),
    .word_ready(p9));
endmodule
