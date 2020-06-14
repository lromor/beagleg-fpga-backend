CXXFLAGS=-Wall -W -Wextra

all: spidev_test beagleg-protocol

beagleg-protocol: beagleg-protocol.o spi.o
	$(CXX) -o $@ $^

spidev_test:

clean:
	rm -f spidev_test beagleg-protocol *.o
