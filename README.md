# An ice40 Beagleg backend replacing a PRU with an FPGA.


## Send bitstream

To send the generated bitstream to the usb-connected FPGA use: `dfu-util -D beagle-spi.dfu`.

# [Simulation](sim/)

# TODOs

## [ ] Spi Protocol

Master initializes the comm. First byte is a command:

COMMANDS:

- NOOP: Receive back the number of free slots we have in fifo.
- STATUS: return as next 4 bytes the status
- FILL_FIFO: The next transaction put all the data in fifo (until channel select is up again)
