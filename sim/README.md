[verilator]: https://www.veripool.org/wiki/verilator
[ice40-spi]: http://www.latticesemi.com/view_document?document_id=50117
[ice40-primitives]: http://www.latticesemi.com/~/media/LatticeSemi/Documents/TechnicalBriefs/SBTICETechnologyLibrary201608.pdf

# Simulation

We are currently concentrating our efforts in having both a hardware and
simulated environment.
To simulate the FPGA dynamics we are using Verilator.
Verilator features a Direct Programming Interface which let's us simulate a
verilog module in c++. 

To simulate the spi connection to the host, we are developing a C++ class
that emulates the host and the internal IP of the ice40.
The SPI ip module is controlled via a verilog primitive `SB_SPI`
([here][ice40-primitives] you can find the datasheet).

# Linking

As pointed out [here](https://github.com/verilator/verilator/issues/2317#issuecomment-626054278), the best way to deal with large projects is to generate separate `libxxx__ALL.a` for each module and link them toghether.

## Run sim

To run the current simulation execute in a terminal `make run`.

## Tasks/Ideas

- [ ] Make test benches prefixed with ct (i.e. ct-my-dpi-interface_test.cc) as "circuit
      testers" to ensure proper port matching between verilog/C++.

# References

- [Ice40 Verilog Primitives][ice40-primitives]
- [Verilator Official Website][verilator]
- [Ice40 SPI IP datasheet][ice40-spi]
