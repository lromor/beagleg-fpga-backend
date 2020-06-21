module SegmentStepGenerator #(
    // we really need some toplevel typedef for the motion segment,
    // so that we don't define this all over everywhere.
    parameter integer READ_BYTES = 4,
    parameter integer PRESCALE_BITS = 23
) (
    input logic clk,

    input  logic                    data_available,
    output logic                    data_request,
    input  logic [8*READ_BYTES-1:0] data,

    output logic step_out
);

  typedef enum {
    STATE_WAIT,
    STATE_EXECUTE
  } state_t;

  // In the lower parts, we have the fast counting bits with
  // system clock frequency; we output the step frequency at that point.
  logic [8*READ_BYTES + PRESCALE_BITS - 1:0] countdown_register;
  assign step_out = countdown_register[PRESCALE_BITS - 1];

  state_t state;
  initial begin
    state = STATE_WAIT;
    countdown_register = 0;
  end

  always @(posedge clk) begin
    case (state)
      STATE_WAIT: begin
        if (data_available) begin
          // available actually means, we can read the data
          // right now.
          countdown_register <= (data << PRESCALE_BITS);
          data_request <= 1'b1;
          state <= STATE_EXECUTE;
        end else begin
          data_request <= 1'b0;
          state <= STATE_WAIT;
          countdown_register = 0;
        end
      end

      STATE_EXECUTE: begin
        data_request <= 1'b0;
        countdown_register <= countdown_register - 1;
        state <= (countdown_register == 0) ? STATE_WAIT : STATE_EXECUTE;
      end

      default: state <= STATE_WAIT;
    endcase  // case (state)
  end
endmodule
