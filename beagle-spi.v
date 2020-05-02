
module top (
   input  clki
   );

   reg        data_trigger; // New data signal
   reg        dir; // Write/read

   always @(posedge data_trigger)
     begin
        // If we receive some data, print hello world.
        \$display("Hello World"); \$finish;
     end

   SB_SPI(
          .SBCLKI(clki),
          .SBRWI(dir),
          .SBSTBI(data_trigger)
          );

endmodule
