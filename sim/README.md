
# Simulation

We are currently concentrating our efforts in having both a hardware and
simulated environment.
To simulate the FPGA dynamics we are using Verilator.
Verilator features a Direct Programming Interface which let's us simulate a
verilog module in c++. 

To simulate the spi connection to the host, we are developing a C++ class
that emulates the host and the internal IP of the ice40.
The SPI ip module is controlled via a verilog primitive `SB_SPI`
([here][ice40-spi] you can find the datasheet).


[verilator]: https://www.veripool.org/wiki/verilator
[ice40-spi]: http://www.latticesemi.com/view_document?document_id=50117

## Run sim

To run the current simulation execute in a terminal `make run`.

# References

- [Verilator Official Website][verilator]
- [Ice40 SPI IP datasheet][ice40-spi]


