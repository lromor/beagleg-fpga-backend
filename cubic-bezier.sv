// Verilog function module which samples
// a third grade (cubic) bezier curve.
// NOTE(lromor): Right now, if we pass s = 1,
// we get the final points inverted. We could fix this
// by adding a condition to the terminal value s = 1.
module cubic_bezier (
    input  signed [16:0] s,
    input  signed [31:0] p1x,
    input  signed [31:0] p1y,
    input  signed [31:0] p1z,
    input  signed [31:0] p2x,
    input  signed [31:0] p2y,
    input  signed [31:0] p2z,
    input  signed [31:0] p3x,
    input  signed [31:0] p3y,
    input  signed [31:0] p3z,
    output signed [17:0] stepsx,
    output signed [17:0] stepsy,
    output signed [17:0] stepsz
);
  wire signed [32:0] s2  /*verilator public_flat*/ = s * s;  // 1 << 32
  wire signed [48:0] s3  /*verilator public_flat*/ = s2 * s;  // 1 << 48
  wire signed [16:0] oms  /*verilator public_flat*/ = (1 << 16) - s;  // 1 << 16
  wire signed [32:0] oms2  /*verilator public_flat*/ = oms * oms;  // 1 << 32

  wire signed [81:0] result[3];

  assign result[0] = 3 * oms2 * s * p1x + 3 * s2 * oms * p2x + p3x * s3;
  assign stepsx = result[0][81 -: 18];

  assign result[1] = 3 * oms2 * s * p1y + 3 * s2 * oms * p2y + p3y * s3;
  assign stepsy = result[1][81 -: 18];

  assign result[2] = 3 * oms2 * s * p1z + 3 * s2 * oms * p2z + p3z * s3;
  assign stepsz = result[2][81 -: 18];
endmodule // cubic_bezier
