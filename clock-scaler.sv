module clock_scaler #(
    parameter integer ScaleFactor = 10
) (
    input  logic clk,
    output logic scaled_clk
);
  if (ScaleFactor == 1) begin : gen_noscaling
    assign scaled_clk = clk;  // Nothing to scale
  end else begin : gen_clock_scaling
    logic [$clog2(ScaleFactor)-1:0] countdown = 0;
    always_ff @(posedge clk) begin
      if (countdown == 1) begin
        countdown  <= ScaleFactor;
        scaled_clk <= 1'b1;
      end else begin
        countdown  <= countdown - 1;
        scaled_clk <= 1'b0;
      end
    end
  end
endmodule
