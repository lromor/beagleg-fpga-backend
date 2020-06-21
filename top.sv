// Should this file be called 'top.v' ?

module top (
    input  clk,
    output system_led,
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

  typedef logic [31:0] MotionSegment;  // Yosys not supported yet: struct

  localparam integer FifoWordSize = 8;
  localparam integer FifoRecordWords = 4;  // Yosys not supported: $bits(MotionSegment) / 8;
  localparam integer FifoDepth = 16;

  typedef enum {
    STATE_IDLE,
    STATE_RECEIVE_SEGMENTS
  } state_e;

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
  } command_e;

  // Spi
  logic [7:0] spi_secondary_data_r;
  wire [7:0] spi_main_data_w;
  wire spi_main_data_ready_w;
  wire [7:0] spi_secondary_data_w;

  // Fifo
  wire [$clog2(FifoDepth * FifoRecordWords):0] fifo_size;
  wire fifo_full_w;
  wire fifo_empty_w;
  wire fifo_write_en;

  // FSM
  state_e state;
  initial state = STATE_IDLE;

  wire [7:0] empty_slots;

  assign empty_slots = FifoDepth - (fifo_size >> $clog2(FifoRecordWords));

  assign spi_secondary_data_w = (state == STATE_IDLE) ? empty_slots : 8'b00000000;
  assign fifo_write_en = (state == STATE_RECEIVE_SEGMENTS) ? spi_main_data_ready_w : 0;

  wire [7:0] debug = {p8, p7, p6, p5, p4, p3, p2, p1};
  assign debug = spi_secondary_data_w;

  // Cyan: empty.
  // Green when fifo has plenty free.
  // Yellow (=red+green) when we only have four left slots.
  // Red when when fifo full and can't accept more.
  // LED signals are negated, as they are using a common anode.
  assign led_blue = !(empty_slots == FifoDepth);
  assign led_green = !(empty_slots > 0);
  assign led_red = !(empty_slots < 4);

  // Receive commands + data from host
  spi_secondary spi_secondary(.clk(clk),
                              .sck(spi_sck),
                              .in_bit(spi_mosi),
                              .out_bit(spi_miso),
                              .cs(spi_cs),
                              .data_word_received(spi_main_data_w),
                              .data_word_to_send(spi_secondary_data_w),
                              .word_ready(spi_main_data_ready_w));

  wire request_read;
  MotionSegment fifo_step_transfer;

  // Motion segments are sent via this fifo to motion engine.
  fifo #(.WordSize(FifoWordSize),
         .RecordWords(FifoRecordWords),
         .Depth(FifoDepth))
   motion_segment_fifo(.clk(clk),
                       .size(fifo_size),
                       // Write stuff
                       .write_en(fifo_write_en),
                       .data_in(spi_main_data_w),
                       // Status
                       .full(fifo_full_w),
                       .empty(fifo_empty_w),
                       // Reading
                       .read_en(request_read),
                       .data_out(fifo_step_transfer));

  segment_step_generator #(.ReadBytes(FifoRecordWords))
   step_gen(.clk(clk),
            .data_available(~fifo_empty_w),
            .data_request(request_read),
            .data(fifo_step_transfer),
            .step_out(system_led));

  // The first byte decides what we're going to do.
  always @(posedge clk) begin
    if (spi_main_data_ready_w & (spi_cs == 0))
      case (state)
        STATE_IDLE: begin
          // Read op
          case (spi_main_data_w)
            CMD_STATUS: state <= STATE_IDLE;  // No-op
            CMD_WRITE_FIFO: state <= STATE_RECEIVE_SEGMENTS;
            default: state <= STATE_IDLE;
          endcase  // case (spi_main_data_w)
        end
        STATE_RECEIVE_SEGMENTS: begin
          // Feeding to fifo do to fifo_write_en. Do nothing otherwise.
        end
        default: state <= STATE_IDLE;  // Do nothing
      endcase

    // Reset the fsm state to idle.
    if (spi_cs) state <= 0;
  end
endmodule
