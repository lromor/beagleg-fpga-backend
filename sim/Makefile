VFLAGS?=-Wno-lint
VINCLUDE?=-Iice40-primitives
VERILOG_SRC_PATH=..
VERILATOR?=verilator
VOUT_DIR=verilator-generated
VERILATOR_INCLUDE_PATH?=$(shell dirname `which verilator`)/../share/verilator/include

# foo.c:5: note: Offset of packed bit-field 'b' has changed in GCC 4.4
# https://gcc.gnu.org/gcc-4.4/changes.html
CFLAGS+=-Wno-packed-bitfield-compat
CFLAGS+=-Wall \
	-I$(VERILATOR_INCLUDE_PATH) \
	-I$(VERILATOR_INCLUDE_PATH)/vltstd

CXXFLAGS+=-std=c++17 $(CFLAGS)
CXX?=g++

VERILATOR_TARGETS=SpiController
VERILATOR_DEPS:=$(VERILATOR_TARGETS:%=V%__ALL.a) $(VERILATOR_TARGETS:%=V%.h)
VERILATOR_DEPS:=$(addprefix $(VOUT_DIR)/, $(VERILATOR_DEPS))

TARGETS=spi-controller_test

all: $(TARGETS)

$(VOUT_DIR)/V%__ALL.a $(VOUT_DIR)/V%.h: $(VERILOG_SRC_PATH)/%.v
	$(VERILATOR) $(VINCLUDE) $(VFLAGS) --Mdir $(VOUT_DIR) --cc $<
	$(MAKE) -C $(VOUT_DIR) -f ../utils/AppendVkObj.mk -f V$*.mk default

verilated-deps: $(VERILATOR_DEPS)

%.o: %.cc verilated-deps
	$(CXX) $(CXXFLAGS)  -c  $< -o $@

spi-controller_test: spi-controller_test.o $(VOUT_DIR)/VSpiController__ALL.a
	$(CXX) -o $@  $^ $(LDFLAGS)

clean:
	rm -rf $(VOUT_DIR) *.o $(TARGETS)

.PHONY: FORCE verilated-deps
