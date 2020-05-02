PCF?=fomu-hacker.pcf
PNRFLAGS?=--up5k --package uwg30
YOSYS?=yosys

all: blink.dfu

# Use *Yosys* to generate the synthesized netlist.
# This is called the **synthesis** and **tech mapping** step.
blink.json: blink.v
	$(YOSYS) -p 'synth_ice40 -top blink -json $@' $^

# Use **nextpnr** to generate the FPGA configuration.
# This is called the **place** and **route** step.
blink.asc: blink.json
	nextpnr-ice40 $(PNRFLAGS) --json blink.json --pcf $(PCF) --asc $@

# Use icepack to convert the FPGA configuration into a "bitstream" loadable onto the FPGA.
# This is called the bitstream generation step.
blink.bit: blink.asc
	icepack $^ $@

blink.dfu: blink.bit
	cp blink.bit blink.dfu
	dfu-suffix -v 1209 -p 70b1 -a blink.dfu

clean:
	rm -rf blink.dfu blink.bit blink.asc blink.json
