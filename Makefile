PCF?=ice40-pinout.pcf
PNRFLAGS?=--up5k --package uwg30
YOSYS?=yosys
TARGET=beagle-spi

all: $(TARGET).dfu

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

%.dfu: %.bit
	cp $< $@
	dfu-suffix -v 1209 -p 70b1 -a $@

clean:
	rm -rf $(TARGET).dfu $(TARGET).bit $(TARGET).asc $(TARGET).json
