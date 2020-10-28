module top (
    input  logic clk,
    output logic system_led,
    output logic led_red,
    output logic led_green,
    output logic led_blue,
    input  logic spi_mosi,
    input  logic spi_sck,
    input  logic spi_cs,
    output logic spi_miso,
    output logic p1,
    output logic p2,
    output logic p3,
    output logic p4,
    output logic p5,
    output logic p6,
    output logic p7,
    output logic p8
);

  localparam integer FifoWordSize = 8;
  // TODO: how can we assert that bits % FifoWordSize == 0 ?
  localparam integer FifoRecordWords = beagleg::MotionSegmentBits / FifoWordSize;
  localparam integer FifoDepth = 16;

  typedef enum {
    STATE_IDLE,
    STATE_RECEIVE_SEGMENTS
  } state_e;

  // Spi
  logic [7:0] spi_secondary_data_r;
  logic [7:0] spi_main_data_w;
  logic spi_main_data_ready_w;
  logic [7:0] spi_secondary_data_w;

  // Fifo
  logic [$clog2(FifoDepth * FifoRecordWords):0] fifo_size;
  logic fifo_full_w;
  logic fifo_empty_w;
  logic fifo_write_en;

  // FSM
  state_e state;
  initial state = STATE_IDLE;

  logic [7:0] empty_slots;

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
  spi_secondary spi_secondary (
      .clk               (clk),
      .sck               (spi_sck),
      .in_bit            (spi_mosi),
      .out_bit           (spi_miso),
      .cs                (spi_cs),
      .data_word_received(spi_main_data_w),
      .data_word_to_send (spi_secondary_data_w),
      .word_ready        (spi_main_data_ready_w)
  );

  logic request_read;
  beagleg::MotionSegment fifo_step_transfer;

  // Motion segments are sent via this fifo to motion engine.
  fifo #(
      .WordSize(FifoWordSize),
      .RecordWords(FifoRecordWords),
      .Depth(FifoDepth)
  ) motion_segment_fifo (
      .clk     (clk),
      .size    (fifo_size),
      // Write stuff
      .write_en(fifo_write_en),
      .data_in (spi_main_data_w),
      // Status
      .full    (fifo_full_w),
      .empty   (fifo_empty_w),
      // Reading
      .read_en (request_read),
      .data_out(fifo_step_transfer)
  );

  segment_step_generator step_gen (
      .clk           (clk),
      .data_available(~fifo_empty_w),
      .data_request  (request_read),
      .data          (fifo_step_transfer),
      .step_out      (system_led)
  );

  cubic_bezier cubic_bezier ();
 
  // The first byte decides what we're going to do.
  always_ff @(posedge clk) begin
    if (spi_main_data_ready_w & (spi_cs == 0))
      case (state)
        STATE_IDLE: begin
          // Read op
          case (spi_main_data_w)
            beagleg::CMD_STATUS: state <= STATE_IDLE;  // No-op
            beagleg::CMD_WRITE_FIFO: state <= STATE_RECEIVE_SEGMENTS;
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
