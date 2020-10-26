// Verilog function module which samples
// a third grade (cubic) bezier curve.
// NOTE(lromor): Right now, if we pass s = 1,
// we get the final points inverted. We could fix this
// by adding a condition to the terminal value s = 1.
module cubic_bezier (
    input  signed [16:0] s,
    input  signed [31:0] p1   [3],
    input  signed [31:0] p2   [3],
    input  signed [31:0] p3   [3],
    output signed [17:0] steps[3]
);
  wire signed [32:0] s2  /*verilator public_flat*/ = s * s;  // 1 << 32
  wire signed [48:0] s3  /*verilator public_flat*/ = s2 * s;  // 1 << 48
  wire signed [16:0] oms  /*verilator public_flat*/ = (1 << 16) - s;  // 1 << 16
  wire signed [32:0] oms2  /*verilator public_flat*/ = oms * oms;  // 1 << 32

  wire signed [81:0] result[3];
  genvar i;
  generate
    for (i = 0; i < 3; i = i + 1) begin: wire_steps
      assign result[i] = 3 * oms2 * s * p1[i] + 3 * s2 * oms * p2[i] + p3[i] * s3;
      assign steps[i] = result[i][81 -: 18];
    end
  endgenerate
endmodule // cubic_bezier
