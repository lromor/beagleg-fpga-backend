VFLAGS?=-Wno-lint --trace
VINCLUDE?=-Iice40-primitives
VERILOG_SRC_PATH=..
VERILATOR?=verilator
VOUT_DIR=verilator-generated
VERILATOR_INCLUDE?=$(shell dirname `which verilator`)/../share/verilator/include

# foo.c:5: note: Offset of packed bit-field 'b' has changed in GCC 4.4
# https://gcc.gnu.org/gcc-4.4/changes.html
CFLAGS+=-Wno-packed-bitfield-compat
CFLAGS+=-Wall -g -O0 \
	-I$(VERILATOR_INCLUDE) \
	-I$(VERILATOR_INCLUDE)/vltstd \
	-I../utils

CXXFLAGS+=-std=c++17 $(CFLAGS)
CXX?=g++

SV_SOURCES=../beagleg-pkg.sv ../spi-secondary.sv ../segment-step-generator.sv ../fifo.sv ../clock-scaler.sv ../top.sv

VERILATOR_TARGETS=$(basename $(notdir $(SV_SOURCES)))
VERILATOR_DEPS:=$(VERILATOR_TARGETS:%=lib%-module.a) $(VERILATOR_TARGETS:%=%-module.h)
VERILATOR_DEPS:=$(addprefix $(VOUT_DIR)/, $(VERILATOR_DEPS))

VERILATOR_XLS_TARGETS=cubic-bezier
VERILATOR_XLS_DEPS:=$(VERILATOR_XLS_TARGETS:%=xls/lib%-module.a) $(VERILATOR_XLS_TARGETS:%=xls/%-module.h)
VERILATOR_XLS_DEPS:=$(addprefix $(VOUT_DIR)/, $(VERILATOR_XLS_DEPS))

COMMON_LIBS=ice40-primitives/libice40dpi.a
MWC=../utils/multi-word-converter

TARGETS=cubic-bezier_test libhsgsim.a

all: verilated-deps $(TARGETS)

ice40-primitives/libice40dpi.a:
	$(MAKE) -C ice40-primitives

generate-xls-sources:
	$(MAKE) -C .. generate-xls-sources

$(VOUT_DIR)/lib%-module.a $(VOUT_DIR)/%-module.h: $(VERILOG_SRC_PATH)/%.sv $(MWC)
	$(eval PASCAL_NAME:=$(shell $(MWC) dash pascal $*))
	$(VERILATOR) $(VINCLUDE) $(VFLAGS) --Mdir $(VOUT_DIR) --prefix V$(PASCAL_NAME) --cc $(SV_SOURCES)
	$(MAKE) -C $(VOUT_DIR) -f ../../utils/AppendVkObj.mk -f V$(PASCAL_NAME).mk default
	mv $(VOUT_DIR)/V$(PASCAL_NAME)__ALL.a $(VOUT_DIR)/lib$*-module.a
	mv $(VOUT_DIR)/V$(PASCAL_NAME).h $(VOUT_DIR)/$*-module.h

# For xls folder
$(VOUT_DIR)/xls/lib%-module.a $(VOUT_DIR)/xls/%-module.h: $(VERILOG_SRC_PATH)/xls/%.sv $(MWC)
	$(eval PASCAL_NAME:=$(shell $(MWC) dash pascal $*))
	@mkdir -p $(VOUT_DIR)
	$(VERILATOR) $(VINCLUDE) $(VFLAGS) --Mdir $(VOUT_DIR)/xls --prefix V$(PASCAL_NAME) --cc $<
	$(MAKE) -C $(VOUT_DIR)/xls -f ../../../utils/AppendVkObj.mk -f V$(PASCAL_NAME).mk default
	mv $(VOUT_DIR)/xls/V$(PASCAL_NAME)__ALL.a $(VOUT_DIR)/xls/lib$*-module.a
	mv $(VOUT_DIR)/xls/V$(PASCAL_NAME).h $(VOUT_DIR)/xls/$*-module.h

verilated-deps: generate-xls-sources $(VERILATOR_DEPS) $(VERILATOR_XLS_DEPS)

%.o: %.cc verilated-deps
	$(CXX) $(CXXFLAGS)  -c  $< -o $@

cubic-bezier_test: cubic-bezier_test.o $(VOUT_DIR)/xls/libcubic-bezier-module.a $(COMMON_LIBS)
	$(CXX) -o $@  $^ $(LDFLAGS)

clock-scaler_test: clock-scaler_test.o libhsgsim.a
	$(CXX) -o $@  $^ $(LDFLAGS)

libhsgsim.a: hsg-sim.o $(VOUT_DIR)/libtop-module.a
	$(AR) rcs $@ $< $(VOUT_DIR)/VTop__ALL.o $(VOUT_DIR)/verilated.o $(VOUT_DIR)/verilated_vcd_c.o

$(MWC):
	$(MAKE) -C ../utils multi-word-converter

clean:
	rm -rf $(VOUT_DIR) *.o $(TARGETS)

.PHONY: FORCE verilated-deps generate-xls-sources
