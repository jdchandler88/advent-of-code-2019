#include <stdio.h>
#include <math.h>

/**
 * Fuel required to launch a given module is based 
 * on its mass. Specifically, to find the fuel required 
 * for a module, take its mass, divide by three, round 
 * down, and subtract 2.
 **/ 
int calculateFuel(int mass) {
    return (int)floor(mass/3) - 2;
}

#ifndef TEST
int main(int argc, char** argv) {
    printf("argc=%i\n", argc);
    for (int i=0; i<argc; i++) {
        printf("arg %i = %s\n", i, argv[i]);
    }
    return 0;
}
#endif