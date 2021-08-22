module segment_step_generator #(
) (
    input  logic                         clk,
    input  logic                         step_sampling_clk,
    input  logic                         data_available,
    output logic                         data_request,
    input  beagleg_pkg::motion_segment_t data,

    output logic step_out,
    output logic is_busy
);

  typedef enum {
    STATE_IDLE,
    STATE_MOVEMENT
  } state_e;

  // Position of the stepper motor, updated with each step sample clk.
  // This is adding up the fractional part, dropping bits overflow on the
  // left.
  logic [63:0] position_accumulator;

  // The step binary output needs to go through one full cycle for each one
  // step update, so we look at the position one right to it.
  assign step_out = position_accumulator[31];

  state_e state;
  assign is_busy  = (state == STATE_MOVEMENT);

  // Mostly for debug.
  assign in_accel = current.current_speed < current.target_speed;

  initial begin
    state = STATE_IDLE;
    position_accumulator = 0;
  end

  beagleg_pkg::motion_segment_t current;

  // TODO: what we actually want is to load the current data with clk
  // speed, so that we can take data off from the fifo as soon as possible

  always_ff @(posedge step_sampling_clk) begin
    if (state == STATE_IDLE && data_available) begin
      current <= data;
      position_accumulator <= 0;
      data_request <= 1'b1;
      state <= STATE_MOVEMENT;
    end else if (state == STATE_MOVEMENT) begin
      // TODO: these things are always off-by-one as the increments are only
      // visible after we clock is finished.
      // What is a common way to properly sequence that ?
      if (current.current_speed < current.target_speed)
        current.current_speed <= current.current_speed + current.current_accel;

      position_accumulator <= position_accumulator + current.current_speed;

      if (position_accumulator[63:32] >= current.target_steps) begin
        position_accumulator <= 0;
        state <= STATE_IDLE;
      end
    end else begin
      data_request <= 1'b0;
    end
  end
endmodule
