PCF?=tinyfpga-bx.pcf
PNRFLAGS?=--lp8k --package cm81

TARGET=BeagleGFPGABackend
SOURCES=BeagleGFPGABackend.v led-blinker.v fifo.v

YOSYS?=yosys

all: $(TARGET).bit

BeagleGFPGABackend.json: $(SOURCES)
	$(YOSYS) -p 'read_verilog -sv $^ ; synth_ice40 -top top -json $@'

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
format: $(SOURCES)
	verilog_format --inplace $^

lint: $(SOURCES)
	verilog_lint $^

clean:
	rm -rf $(TARGET).dfu $(TARGET).bit $(TARGET).asc $(TARGET).json

.PHONY: flash
