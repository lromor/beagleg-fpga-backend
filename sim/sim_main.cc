
#include <stdio.h>
#include "BeagleSpi.h"
#include "verilated.h"
#include "BeagleSpi__Dpi.h"

void trigger(unsigned char clki, unsigned char *a) {
    *a ^= clki;
}

int main(int argc, char** argv, char** env) {
    Verilated::commandArgs(argc, argv);
    BeagleSpi * top = new BeagleSpi;
    while (!Verilated::gotFinish()) {
        top->clki ^= 1;
        top->eval();
    }
    delete top;
    return 0;
}
