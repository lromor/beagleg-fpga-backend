[optimal-feed-interpolator-g2-bezier]: https://link.springer.com/article/10.1186/s10033-019-0360-8
[realtime-interpolator-for-parametric-curves]: https://pureportal.strath.ac.uk/files/70894142/Zhong_etal_IJMTM_201_A_real_time_interpolator_for_parametric_curves.pdf
[curvature-optimal-sharp-corner-smoothing]: https://link.springer.com/article/10.1007/s00170-014-6386-2


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


# Formal verification

To run symbiosis, use `sby -f fifo.sby`

# Planning-related research papers

- [An Optimal Feed Interpolator Based on G2 Continuous Bézier Curves for High-Speed Machining of Linear Tool Path][optimal-feed-interpolator-g2-bezier]
- [A real-time interpolator for parametric curves][realtime-interpolator-for-parametric-curves]
- [A curvature optimal sharp corner smoothing algorithm for high-speed feed motion generation of NC systems along linear tool paths][curvature-optimal-sharp-corner-smoothing]

