GTEST_SOURCE?=/usr/src/gtest
GMOCK_SOURCE?=/usr/src/gmock

GTEST_INCLUDE=-I$(GTEST_SOURCE)/include -I$(GMOCK_SOURCE)/include

VFLAGS?=-Wno-lint
VINCLUDE?=-Iice40-primitives
VERILOG_SRC_PATH=.
VERILATOR?=verilator
VOUT_DIR=verilator-generated
VERILATOR_INCLUDE?=$(shell dirname `which verilator`)/../share/verilator/include

CFLAGS+=-Wall -g -O0
CFLAGS+=-Wno-packed-bitfield-compat
CFLAGS+=-I$(VERILATOR_INCLUDE) -I$(VERILATOR_INCLUDE)/vltstd

# See toplevel Makefile comment
CXXFLAGS+=-std=c++17 $(CFLAGS)
CXX?=g++

LDFLAGS+=-lpthread -lm

OBJECTS=sb-spi.o
GENLIB=libice40dpi.a

VERILATOR_TARGETS=SB_SPI
VERILATOR_DEPS:=$(VERILATOR_TARGETS:%=V%__ALL.a) $(VERILATOR_TARGETS:%=V%.h)
VERILATOR_DEPS:=$(addprefix $(VOUT_DIR)/, $(VERILATOR_DEPS))

UNITTEST_BINARIES=sb-spi_test
TEST_FRAMEWORK_OBJECTS=gtest-all.o gmock-all.o

DEPENDENCY_RULES=$(OBJECTS:=.d) $(UNITTEST_BINARIES:=.o.d)

all : $(GENLIB)

$(GENLIB): $(OBJECTS)
	$(AR) rcs $@ $^

test: $(UNITTEST_BINARIES)
	for test_bin in $(UNITTEST_BINARIES) ; do echo ; echo $$test_bin; ./$$test_bin || exit 1 ; done

valgrind-test: $(UNITTEST_BINARIES)
	for test_bin in $(UNITTEST_BINARIES) ; do valgrind --track-origins=yes --leak-check=full --error-exitcode=1 -q ./$$test_bin || exit 1; done

$(VOUT_DIR)/V%__ALL.a $(VOUT_DIR)/V%.h: $(VERILOG_SRC_PATH)/%.v
	$(VERILATOR) $(VINCLUDE) $(VFLAGS) --Mdir $(VOUT_DIR) --cc $<
	$(MAKE) -C $(VOUT_DIR) -f ../../utils/AppendVkObj.mk -f V$*.mk default

verilated-deps: $(VERILATOR_DEPS)

%_test: %_test.o $(GENLIB) $(TEST_FRAMEWORK_OBJECTS) compiler-flags
	$(CXX) -o $@ $< $(GENLIB) $(LDFLAGS) $(TEST_FRAMEWORK_OBJECTS)

%.o: %.cc compiler-flags verilated-deps
	$(CXX) $(CXXFLAGS)  -c  $< -o $@
	@$(CXX) $(CXXFLAGS) -MM $< > $@.d

%_test.o: %_test.cc compiler-flags
	$(CXX)  $(GTEST_INCLUDE) $(CXXFLAGS) -c  $< -o $@
	@$(CXX) $(GTEST_INCLUDE) $(CXXFLAGS) -MM $< > $@.d

# Auto generated dependencies
-include $(DEPENDENCY_RULES)

# Test and Mock framework.
gtest-all.o : $(GTEST_SOURCE)/src/gtest-all.cc compiler-flags
	$(CROSS_COMPILE)$(CXX) $(CXXFLAGS) $(GTEST_INCLUDE) -I$(GTEST_SOURCE) -I$(GTEST_SOURCE)/include -c  $< -o $@

gmock-all.o : $(GMOCK_SOURCE)/src/gmock-all.cc compiler-flags
	$(CROSS_COMPILE)$(CXX) $(CXXFLAGS) $(GTEST_INCLUDE) -I$(GMOCK_SOURCE) -I$(GMOCK_SOURCE)/include -c  $< -o $@

clean:
	rm -rf $(GENLIB) $(VOUT_DIR) $(MAIN_OBJECTS) $(OBJECTS) $(UNITTEST_BINARIES) $(UNITTEST_BINARIES:=.o) $(DEPENDENCY_RULES) $(TEST_FRAMEWORK_OBJECTS) *.gcda *.gcov *.gcno *.cc.html *.h.html

compiler-flags: FORCE
	@echo '$(CXX) $(CXXFLAGS) $(GTEST_INCLUDE)' | cmp -s - $@ || echo '$(CXX) $(CXXFLAGS) $(GTEST_INCLUDE)' > $@

.PHONY: FORCE verilated-deps
