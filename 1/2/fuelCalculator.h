#include <stdio.h>
#include <math.h>
#include <utils.h>
#include <stdlib.h>

/**
 * Fuel required to launch a given module is based 
 * on its mass. Specifically, to find the fuel required 
 * for a module, take its mass, divide by three, round 
 * down, and subtract 2.
 * 
 * So, for each module mass, calculate its fuel and add it 
 * to the total. Then, treat the fuel amount you just 
 * calculated as the input mass and repeat the process, 
 * continuing until a fuel requirement is zero or negative.
 **/ 
int calculateFuel(int mass) {
    int fuel = 0;
    int moduleFuel; 
    int moduleMass = mass; 
    while ( (moduleFuel = (int)floor(moduleMass/3) - 2) > 0) {
        fuel+=moduleFuel;  
        moduleMass = moduleFuel;  //store fuel as mass and potentially calculate more fuel for this fuel
    }
    return fuel;
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