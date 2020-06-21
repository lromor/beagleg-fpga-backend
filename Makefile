PCF?=tinyfpga-bx.pcf
PNRFLAGS?=--lp8k --package cm81

TARGET=beagleg-fpga-backend
SOURCES=beagleg-fpga-backend.v fifo.v spi-secondary.v

YOSYS?=yosys

all: $(TARGET).bit

beagleg-fpga-backend.json: $(SOURCES)
	!($(YOSYS) -p 'read_verilog -sv $^ ; synth_ice40 -top top -json $@' 2>&1 | egrep -i "(^warning:|error:)") || rm $@

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
# (a somewhat recent version, as the binary name changed)
format: $(SOURCES)
	verible-verilog-format --inplace $^

lint: $(SOURCES)
	verible-verilog-lint $^

clean:
	rm -rf $(TARGET).dfu $(TARGET).bit $(TARGET).asc $(TARGET).json

.PHONY: flash
