
module SpiController (
  input  clki
  );
  SB_SPI spi (
    .SBCLKI(clki)
  );
endmodule
