CXXFLAGS=-Wall -W -Wextra -std=c++17 -I/usr/local/share/verilator/include

all: spidev_test beagleg-protocol

../sim/libhsgsim.a: FORCE
	$(MAKE) -C ../sim libhsgsim.a

%.o: %.cc ../sim/libhsgsim.a
	$(CXX) $(CXXFLAGS)  -c  $< -o $@

beagleg-protocol: beagleg-protocol.o spi.o ../sim/libhsgsim.a
	$(CXX) -o $@ $^

spidev_test:

clean:
	rm -f spidev_test beagleg-protocol *.o

.PHONY: FORCE
