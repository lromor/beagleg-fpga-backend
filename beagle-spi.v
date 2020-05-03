
/* verilator lint_off DECLFILENAME */
module top (
   input  clki
   );

   reg    data_trigger;

`ifdef VERILATOR
   // At some point the strobe will trigger pringint hello world
   always @(posedge data_trigger)
     begin
        // If we receive some data, print hello world.
        $display("Hello World");
        $finish;
     end
`endif

   SB_SPI spi (
      .SBCLKI(clki),
      .SBSTBI(data_trigger)
      );
  

endmodule
