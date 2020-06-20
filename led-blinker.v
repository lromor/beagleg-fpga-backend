// Purely testing LED blinking right now :)
module LedBlinker (
    input  clk,
    output led_red,
    output led_green,
    output led_blue
);

  reg [24:0] prescaler;
  always @(posedge clk) begin
    prescaler <= prescaler + 1;
  end

  // Let's assign to one of the LEDs. They have
  // a common anode, so '0' means they are on.
  assign led_red = 1;
  assign led_blue = prescaler[24];
  assign led_green = 1;
endmodule  // LedBlinker
