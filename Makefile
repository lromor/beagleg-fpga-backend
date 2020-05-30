PCF?=ice40-pinout.pcf
PNRFLAGS?=--lp8k --package cm81

YOSYS?=yosys
TARGET=BeagleGFPGABackend

all: $(TARGET).bit

# Use *Yosys* to generate the synthesized netlist.
# This is called the **synthesis** and **tech mapping** step.
%.json: %.v
	$(YOSYS) -p 'synth_ice40 -top top -json $@' $<

# Use **nextpnr** to generate the FPGA configuration.
# This is called the **place** and **route** step.
%.asc: %.json
	nextpnr-ice40 $(PNRFLAGS) --json $< --pcf $(PCF) --asc $@

# Use icepack to convert the FPGA configuration into a "bitstream" loadable onto the FPGA.
# This is called the bitstream generation step.
%.bit: %.asc
	icepack $< $@

flash: $(TARGET).bit
	tinyprog -p $<

# needs https://github.com/google/verible
format: $(TARGET).sv $(TARGET).v SpiController.v
	verilog_format --inplace $<

clean:
	rm -rf $(TARGET).dfu $(TARGET).bit $(TARGET).asc $(TARGET).json

.PHONY: flash
