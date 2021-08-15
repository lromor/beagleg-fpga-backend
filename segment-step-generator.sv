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
  // This is a 16.16 fixed point value, so the actual position is at
  // 31:16.
  logic [31:0] position_accumulator;

  // The step binary output needs to go through one full cycle for each one
  // step update, so we look at the position one right to it.
  assign step_out = position_accumulator[15];

  state_e state;
  assign is_busy = (state == STATE_MOVEMENT);

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
      position_accumulator <= position_accumulator + current.delta_distance_per_sample;
      current.sample_count <= current.sample_count - 1;
      if (current.sample_count == 1) begin
        position_accumulator <= 0;
        state <= STATE_IDLE;
      end
    end else begin
      data_request <= 1'b0;
    end
  end
endmodule
