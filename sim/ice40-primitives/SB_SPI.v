import "DPI-C" function void trigger(output logic a);

/* verilator lint_off UNUSED */
/* verilator lint_off UNDRIVEN */
module SB_SPI (
	input  SBCLKI,
	input  SBRWI,
	output  SBSTBI,
	input  SBADRI7,
	input  SBADRI6,
	input  SBADRI5,
	input  SBADRI4,
	input  SBADRI3,
	input  SBADRI2,
	input  SBADRI1,
	input  SBADRI0,
	input  SBDATI7,
	input  SBDATI6,
	input  SBDATI5,
	input  SBDATI4,
	input  SBDATI3,
	input  SBDATI2,
	input  SBDATI1,
	input  SBDATI0,
	input  MI,
	input  SI,
	input  SCKI,
	input  SCSNI,
	output SBDATO7,
	output SBDATO6,
	output SBDATO5,
	output SBDATO4,
	output SBDATO3,
	output SBDATO2,
	output SBDATO1,
	output SBDATO0,
	output SBACKO,
	output SPIIRQ,
	output SPIWKUP,
	output SO,
	output SOE,
	output MO,
	output MOE,
	output SCKO, //inout in the SB verilog library, but output in the VHDL and PDF libs and seemingly in the HW itself
	output SCKOE,
	output MCSNO3,
	output MCSNO2,
	output MCSNO1,
	output MCSNO0,
	output MCSNOE3,
	output MCSNOE2,
	output MCSNOE1,
	output MCSNOE0
);
   parameter BUS_ADDR74 = "0b0000";
   initial begin
	  trigger(SBSTBI);
	end
   //initial begin
//	  $display("%x + %x = %x", 1, 2, trigger(1));
 //  end
endmodule
/* verilator lint_on UNUSED */
/* verilator lint_on UNDRIVEN */
