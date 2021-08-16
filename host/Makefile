# If simulation is switched on, beagleg-protocol doesn't actually talk to
# an SPI bus but to the Verilator-simulated backend.
USE_SIMULATION=1

CXXFLAGS=-Wall -W -Wextra -I../sim -I/usr/local/share/verilator/include -std=c++17

SIM_LIB=../sim/libhsgsim.a

ifeq ($(USE_SIMULATION), 1)
  CXXFLAGS += -DUSE_SIMULATION=1
  EXTRA_DEPENDENCIES=$(SIM_LIB)
endif

all: spidev_test beagleg-protocol

$(SIM_LIB): PHONY
	$(MAKE) -C ../sim libhsgsim.a

beagleg-protocol.o: $(EXTRA_DEPENDENCIES)
spi.o: $(EXTRA_DEPENDENCIES)

beagleg-protocol: beagleg-protocol.o spi.o $(EXTRA_DEPENDENCIES)
	$(CXX) -o $@ $^

spidev_test:

clean:
	$(MAKE) -C ../sim clean
	rm -f spidev_test beagleg-protocol *.o

PHONY:
