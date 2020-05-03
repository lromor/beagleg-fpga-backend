
#include "BeagleSpi.h"
#include "verilated.h"
#include <iostream>
#include "BeagleSpi__Dpi.h"

void trigger(unsigned char clki, unsigned char *a) {
   // Immediately trigger
   std::cout << "CYCLE!" << std::endl;
   *a = 1;
}

int main(int argc, char** argv, char** env) {
    Verilated::commandArgs(argc, argv);
    BeagleSpi * top = new BeagleSpi;
    while (!Verilated::gotFinish()) {
        top->clki++;
        top->eval();
    }
    delete top;
    return 0;
}
