

# Run sim
First compile

``` sh
verilator -Iice40-primitives  -Wno-lint --prefix BeagleSpi  --cc ../beagle-spi.v --exe sim_main.cpp  && make -j -C obj_dir -f BeagleSpi.mk BeagleSpi
```

then execute `./obj_dir/BeagleSpi`
