CXXFLAGS=-Wall -W -Wextra -I../sim -I/usr/local/share/verilator/include -std=c++17

SIM_LIB=../sim/libhsgsim.a

all: spidev_test beagleg-protocol

$(SIM_LIB): PHONY
	$(MAKE) -C ../sim libhsgsim.a

beagleg-protocol: beagleg-protocol.o spi.o $(SIM_LIB)
	$(CXX) -o $@ $^ $(SIM_LIB)

spidev_test:

clean:
	$(MAKE) -C ../sim clean
	rm -f spidev_test beagleg-protocol *.o

PHONY:
