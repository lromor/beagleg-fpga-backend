PCF?=tinyfpga-bx.pcf
PNRFLAGS?=--lp8k --package cm81

TARGET=beagleg-fpga-backend
MANUAL_SOURCES=beagleg-pkg.sv fifo.sv spi-secondary.sv segment-step-generator.sv clock-scaler.sv top.sv
XLS_SOURCES=xls/cubic-bezier.sv
SOURCES=$(MANUAL_SOURCES) $(XLS_SOURCES)

SURELOG?=surelog

YOSYS?=yosys

all: $(TARGET).bit

# Compile xls generated verilog.
$(XLS_SOURCES):
	$(MAKE) -C xls

generate-xls-sources: $(XLS_SOURCES)

# Run yosys, but also make sure we fail even on warnings.
$(TARGET).json: $(SOURCES)
	!($(YOSYS) -p 'read_verilog -sv $^ ; synth_ice40 -top top -json $@' 2>&1 | egrep -i "(^warning:|error:)") || rm $@

# Use **nextpnr** to generate the FPGA configuration.
# This is called the **place** and **route** step.
%.asc: %.json $(PCF)
	nextpnr-ice40 $(PNRFLAGS) --json $< --pcf $(PCF) --asc $@

# Use icepack to convert the FPGA configuration into a "bitstream" loadable onto the FPGA.
# This is called the bitstream generation step.
%.bit: %.asc
	icepack $< $@

flash: $(TARGET).bit
	tinyprog -p $<

# needs https://github.com/google/verible
# (a somewhat recent version, as the binary name changed)
format: $(MANUAL_SOURCES)
	verible-verilog-format --inplace $^

lint: $(MANUAL_SOURCES)
	verible-verilog-lint --rules_config_search $^

# test-run with surelog to see if there are more error messages.
surelog: $(SOURCES)
	$(SURELOG) -parse -sv $^

clean:
	rm -rf $(TARGET).dfu $(TARGET).bit $(TARGET).asc $(TARGET).json

verify-formal:
	sby -t -f fifo.sby

.PHONY: flash generate-xls-sources
