typedef struct packed {
  logic SBCLKI;
  logic SBRWI;
  logic SBSTBI;
  logic [7:0] SBADRI;
  logic [7:0] SBDATI;
  logic MI;
  logic SI;
  logic SCKI;
  logic SCSNI;
} DpiInputs;

typedef struct packed {
  logic [7:0] SBDATO;
  logic SBACKO;
  logic SPIIRQ;
  logic SPIWKUP;
  logic SO;
  logic SOE;
  logic MO;
  logic MOE;
  logic SCKO;
  logic SCKOE;
  logic [3:0] MCSNO;
  logic [3:0] MCSNOE;
} DpiOutputs;


import "DPI-C" function void
  sb_spi_dpi(
    input DpiInputs inputs,
    output DpiOutputs outputs
    );

/* verilator lint_off UNUSED */
/* verilator lint_off UNDRIVEN */
module SB_SPI (
	input  SBCLKI,
	input  SBRWI,
	input  SBSTBI,
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
	output SCKO,
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
  wire [7:0] sbadri = {SBADRI7, SBADRI6, SBADRI5, SBADRI4, SBADRI3, SBADRI2, SBADRI1, SBADRI0};
  wire [7:0] sbdati = {SBDATI7, SBDATI6, SBDATI5, SBDATI4, SBDATI3, SBDATI2, SBDATI1, SBDATI0};
  wire [7:0] sbdato = {SBDATO7, SBDATO6, SBDATO5, SBDATO4, SBDATO3, SBDATO2, SBDATO1, SBDATO0};
  wire [3:0] mcsno = {MCSNO3, MCSNO2, MCSNO1, MCSNO0};
  wire [3:0] mcsnoe = {MCSNOE3, MCSNOE2, MCSNOE1, MCSNOE0};
  DpiInputs dpii = {SBCLKI, SBRWI, SBSTBI, sbadri, sbdati, MI, SI, SCKI, SCSNI};
  DpiOutputs dpio = {sbdato, SBACKO, SPIIRQ, SPIWKUP, SO, SOE, MO, MOE, SCKO, SCKOE, mcsno, mcsnoe};

  always_comb
    sb_spi_dpi(dpii, dpio);
endmodule
