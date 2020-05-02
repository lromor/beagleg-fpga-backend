PCF?=fomu-hacker.pcf
PNRFLAGS?=--up5k --package uwg30
YOSYS?=yosys

blink.json: blink.v
	$(YOSYS) -p 'synth_ice40 -top blink -json $@' $^

blink.asc: blink.json
	nextpnr-ice40 $(PNRFLAGS) --json blink.json --pcf $(PCF) --asc $@

blink.bin: blink.asc
	icepack $^ $@

clean:
	rm -rf blink.bin blink.asc blink.json
