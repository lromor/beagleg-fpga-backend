module __cubic_bezier_private__main (
    input wire clk,
    input wire [16:0] x,
    input wire [22:0] a,
    input wire [22:0] b,
    input wire [22:0] c,
    output wire [73:0] out
);
  // lint_off SIGNED_TYPE
  // lint_off MULTIPLY
  function automatic [33:0] smul34b_17b_x_17b(input reg [16:0] lhs, input reg [16:0] rhs);
    reg signed [16:0] signed_lhs;
    reg signed [16:0] signed_rhs;
    reg signed [33:0] signed_result;
    begin
      signed_lhs = $signed(lhs);
      signed_rhs = $signed(rhs);
      signed_result = signed_lhs * signed_rhs;
      smul34b_17b_x_17b = $unsigned(signed_result);
    end
  endfunction
  // lint_on MULTIPLY
  // lint_on SIGNED_TYPE
  // lint_off SIGNED_TYPE
  // lint_off MULTIPLY
  function automatic [56:0] smul57b_23b_x_34b(input reg [22:0] lhs, input reg [33:0] rhs);
    reg signed [22:0] signed_lhs;
    reg signed [33:0] signed_rhs;
    reg signed [56:0] signed_result;
    begin
      signed_lhs = $signed(lhs);
      signed_rhs = $signed(rhs);
      signed_result = signed_lhs * signed_rhs;
      smul57b_23b_x_34b = $unsigned(signed_result);
    end
  endfunction
  // lint_on MULTIPLY
  // lint_on SIGNED_TYPE
  // lint_off SIGNED_TYPE
  // lint_off MULTIPLY
  function automatic [39:0] smul40b_23b_x_17b(input reg [22:0] lhs, input reg [16:0] rhs);
    reg signed [22:0] signed_lhs;
    reg signed [16:0] signed_rhs;
    reg signed [39:0] signed_result;
    begin
      signed_lhs = $signed(lhs);
      signed_rhs = $signed(rhs);
      signed_result = signed_lhs * signed_rhs;
      smul40b_23b_x_17b = $unsigned(signed_result);
    end
  endfunction
  // lint_on MULTIPLY
  // lint_on SIGNED_TYPE
  // lint_off SIGNED_TYPE
  // lint_off MULTIPLY
  function automatic [50:0] smul51b_34b_x_17b(input reg [33:0] lhs, input reg [16:0] rhs);
    reg signed [33:0] signed_lhs;
    reg signed [16:0] signed_rhs;
    reg signed [50:0] signed_result;
    begin
      signed_lhs = $signed(lhs);
      signed_rhs = $signed(rhs);
      signed_result = signed_lhs * signed_rhs;
      smul51b_34b_x_17b = $unsigned(signed_result);
    end
  endfunction
  // lint_on MULTIPLY
  // lint_on SIGNED_TYPE
  // lint_off SIGNED_TYPE
  // lint_off MULTIPLY
  function automatic [73:0] smul74b_23b_x_51b(input reg [22:0] lhs, input reg [50:0] rhs);
    reg signed [22:0] signed_lhs;
    reg signed [50:0] signed_rhs;
    reg signed [73:0] signed_result;
    begin
      signed_lhs = $signed(lhs);
      signed_rhs = $signed(rhs);
      signed_result = signed_lhs * signed_rhs;
      smul74b_23b_x_51b = $unsigned(signed_result);
    end
  endfunction
  // lint_on MULTIPLY
  // lint_on SIGNED_TYPE

  // ===== Pipe stage 0:

  // Registers for pipe stage 0:
  reg [16:0] p0_x;
  reg [22:0] p0_a;
  reg [22:0] p0_b;
  reg [22:0] p0_c;
  always_ff @(posedge clk) begin
    p0_x <= x;
    p0_a <= a;
    p0_b <= b;
    p0_c <= c;
  end

  // ===== Pipe stage 1:
  wire [33:0] p1_x2_comb;
  wire [56:0] p1_p2_comb;
  wire [39:0] p1_p1_comb;
  wire [40:0] p1_bit_slice_99_comb;
  wire [50:0] p1_x3_comb;
  wire [41:0] p1_add_84_comb;
  wire [73:0] p1_p3_comb;
  wire [57:0] p1_add_96_comb;
  wire [73:0] p1_concat_98_comb;
  assign p1_x2_comb = smul34b_17b_x_17b(p0_x, p0_x);
  assign p1_p2_comb = smul57b_23b_x_34b(p0_b, p1_x2_comb);
  assign p1_p1_comb = smul40b_23b_x_17b(p0_a, p0_x);
  assign p1_bit_slice_99_comb = p1_p2_comb[56:16];
  assign p1_x3_comb = smul51b_34b_x_17b(p1_x2_comb, p0_x);
  assign p1_add_84_comb = {{2{p1_p1_comb[39]}}, p1_p1_comb} + {{1{p1_bit_slice_99_comb[40]}}, p1_bit_slice_99_comb};
  assign p1_p3_comb = smul74b_23b_x_51b(p0_c, p1_x3_comb);
  assign p1_add_96_comb = {p1_add_84_comb, p1_p2_comb[15:0]} + p1_p3_comb[73:16];
  assign p1_concat_98_comb = {p1_add_96_comb, p1_p3_comb[15:0]};

  // Registers for pipe stage 1:
  reg [73:0] p1_concat_98;
  always_ff @(posedge clk) begin
    p1_concat_98 <= p1_concat_98_comb;
  end
  assign out = p1_concat_98;
endmodule
