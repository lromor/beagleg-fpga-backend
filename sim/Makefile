
TARGET=BeagleSpi
VFLAGS?=-Wno-lint --prefix $(TARGET)
VINCLUDE?=-Iice40-primitives
VERILOG_SRC=../beagle-spi.v
SIM_SRC=sim_main.cc
VERILATOR?=verilator
VOUT_DIR=obj_dir

all: $(TARGET)

$(VOUT_DIR): $(VERILOG_SRC) $(SIM_SRC)
	$(VERILATOR) $(VINCLUDE) $(VFLAGS) \
		--cc $(VERILOG_SRC) --exe $(SIM_SRC) -o ../$(TARGET)

$(TARGET): $(VOUT_DIR)
	$(MAKE) -j -C $(VOUT_DIR) -f $(TARGET).mk $(TARGET)

clean:
	rm -rf $(VOUT_DIR)
