#include <stdio.h>
#include <math.h>
#include <utils.h>
#include <stdlib.h>

/**
 * Fuel required to launch a given module is based 
 * on its mass. Specifically, to find the fuel required 
 * for a module, take its mass, divide by three, round 
 * down, and subtract 2.
 **/ 
int calculateFuel(int mass) {
    return (int)floor(mass/3) - 2;
}

static int fuel = 0;
void accumulateMass(const char* massString) {
    int mass = atoi(massString);
    fuel += calculateFuel(mass);
}

#ifndef TEST
int main(int argc, char** argv) {
    if (argc <= 1) {
        return 1;   //no file provided
    }
    //just assume a file. we would be robust if this were for production
    FILE* f = fopen(argv[1], "r");
    readLines(f, accumulateMass);
    fclose(f);
    printf("total fuel = %i\n", fuel);
    return 0;
}
#endif