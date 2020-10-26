KEBAB2PASCAL=utils/kebab2pascal

VFLAGS?=-Wno-lint
VINCLUDE?=-Iice40-primitives
VERILOG_SRC_PATH=..
VERILATOR?=verilator
VOUT_DIR=verilator-generated
VERILATOR_INCLUDE?=$(shell dirname `which verilator`)/../share/verilator/include

# foo.c:5: note: Offset of packed bit-field 'b' has changed in GCC 4.4
# https://gcc.gnu.org/gcc-4.4/changes.html
CFLAGS+=-Wno-packed-bitfield-compat
CFLAGS+=-Wall \
	-I$(VERILATOR_INCLUDE) \
	-I$(VERILATOR_INCLUDE)/vltstd \
	-Ifpm/include

CXXFLAGS+=-std=c++17 $(CFLAGS)
CXX?=g++

VERILATOR_TARGETS=cubic-bezier
VERILATOR_DEPS:=$(VERILATOR_TARGETS:%=lib%-module.a) $(VERILATOR_TARGETS:%=%-module.h)
VERILATOR_DEPS:=$(addprefix $(VOUT_DIR)/, $(VERILATOR_DEPS))

COMMON_LIBS=ice40-primitives/libice40dpi.a

TARGETS=cubic-bezier_test

all: verilated-deps $(TARGETS)

ice40-primitives/libice40dpi.a:
	$(MAKE) -C ice40-primitives

$(VOUT_DIR)/lib%-module.a $(VOUT_DIR)/%-module.h: $(VERILOG_SRC_PATH)/%.sv $(KEBAB2PASCAL)
	$(eval PASCAL_NAME:=$(shell $(KEBAB2PASCAL) $*))
	$(VERILATOR) $(VINCLUDE) $(VFLAGS) --Mdir $(VOUT_DIR) --prefix V$(PASCAL_NAME) --cc $<
	$(MAKE) -C $(VOUT_DIR) -f ../utils/AppendVkObj.mk -f V$(PASCAL_NAME).mk default
	mv $(VOUT_DIR)/V$(PASCAL_NAME)__ALL.a $(VOUT_DIR)/lib$*-module.a
	mv $(VOUT_DIR)/V$(PASCAL_NAME).h $(VOUT_DIR)/$*-module.h

verilated-deps: $(VERILATOR_DEPS)

%.o: %.cc verilated-deps
	$(CXX) $(CXXFLAGS)  -c  $< -o $@

cubic-bezier_test: cubic-bezier_test.o $(VOUT_DIR)/libcubic-bezier-module.a $(COMMON_LIBS)
	$(CXX) -o $@  $^ $(LDFLAGS)

$(KEBAB2PASCAL):
	$(MAKE) -C utils kebab2pascal

clean:
	rm -rf $(VOUT_DIR) *.o $(TARGETS)

.PHONY: FORCE verilated-deps
