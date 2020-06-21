// Should this file be called 'top.v' ?

module top (
    input  clk,
    output led_red,
    output led_green,
    output led_blue,
    input  spi_mosi,
    input  spi_sck,
    input  spi_cs,
    output spi_miso,
    output p1,
    output p2,
    output p3,
    output p4,
    output p5,
    output p6,
    output p7,
    output p8
);

  localparam integer FIFO_WORD_SIZE = 8;
  localparam integer FIFO_RECORD_WORDS = 4;
  localparam integer FIFO_SLOTS = 16;

  typedef enum {
    STATE_IDLE,
    STATE_RECEIVE_SEGMENTS
  } state_t;

  // Must match enum in beagleg-protocol.cc
  // (comments written like that because
  // https://github.com/google/verible/issues/336 )
  typedef enum {
    // Get fifo free slots
    CMD_NO_OP = 0,

    // Get status word
    CMD_STATUS = 1,

    // Send segments to fifo
    CMD_WRITE_FIFO = 2
  } command_t;

  LedBlinker blinker(.clk(clk),
                     .led_red(led_red),
                     .led_green(led_green),
                     .led_blue(led_blue));

  // Spi
  reg [7:0] spi_secondary_data_r;
  wire [7:0] spi_main_data_w;
  wire spi_main_data_ready_w;
  wire [7:0] spi_secondary_data_w;

  // Fifo
  wire [$clog2(FIFO_SLOTS * FIFO_RECORD_WORDS):0] fifo_size;
  wire fifo_full_w;
  wire fifo_empty_w;
  wire fifo_write_en;

  // FSM
  state_t state;
  initial state = STATE_IDLE;

  assign spi_secondary_data_w = (state == STATE_IDLE) ?
      FIFO_SLOTS - (fifo_size >> $clog2(FIFO_RECORD_WORDS)) : 8'b00000000;
  assign fifo_write_en = (state == STATE_RECEIVE_SEGMENTS) ? spi_main_data_ready_w : 0;

  wire [7:0] debug = {p8, p7, p6, p5, p4, p3, p2, p1};

  // Use the fifo as buffer for the data collected
  // from the spi.
  Fifo #(.WORD_SIZE(FIFO_WORD_SIZE),
         .RECORD_WORDS(FIFO_RECORD_WORDS),
         .SLOTS(FIFO_SLOTS)) fifo(.clk(clk),
                                  .size(fifo_size),
                                  // Write stuff
                                  .write_en(fifo_write_en),
                                  .data_in(spi_main_data_w),
                                  // Status
                                  .full(fifo_full_w),
                                  .empty(fifo_empty_w),
                                  // Read stuff
                                  .read_en(deplete_r),
                                  .data_out(fifo_out));

  SpiSecondary spi_secondary(.clk(clk),
                             .sck(spi_sck),
                             .in_bit(spi_mosi),
                             .out_bit(spi_miso),
                             .cs(spi_cs),
                             .data_word_received(spi_main_data_w),
                             .data_word_to_send(spi_secondary_data_w),
                             .word_ready(spi_main_data_ready_w));

  always @(posedge clk) begin
    debug <= spi_secondary_data_w;
    if (spi_main_data_ready_w & (spi_cs == 0))
      case (state)
        STATE_IDLE: begin
          // Read op
          case (spi_main_data_w)
            CMD_STATUS: state <= STATE_IDLE;  // No-op
            CMD_WRITE_FIFO: state <= STATE_RECEIVE_SEGMENTS;
          endcase  // case (spi_main_data_w)
        end
        STATE_RECEIVE_SEGMENTS: begin
          // Feeding to fifo. Do nothing?
        end
        default: state <= STATE_IDLE;  // Do nothing
      endcase

    // Reset the fsm state to idle.
    if (spi_cs) state <= 0;
  end
endmodule
