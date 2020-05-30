// Should this file be called 'top.v' ?

module top (input clk,
            output led_red,
	    output led_green,
	    output led_blue);

  LedBlinker blinker(.clk(clk),
		     .led_red(led_red),
		     .led_green(led_green),
		     .led_blue(led_blue));
endmodule
