#include "sol.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int numRoutes = 0;
const char* route1;
const char* route2;

static void programStringCallback(const char* routeString) {
    numRoutes++;
    switch (numRoutes) {
        case 1:
            route1 = copyString(routeString);
            break;
        case 2:
            route2 = copyString(routeString);
            break;
    }
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("bruh. gimme an input file.\n");
        exit(0);
    }
    FILE* file = fopen(argv[1], "r");
    readLines(file, programStringCallback);
    if (numRoutes != 2) {
        printf("yikes, received %i routes but expected 2. exiting.\n", numRoutes);
        exit(1);
    }
    int minDistance = calculateMinIntersectionStepsFromRouteStrings(route1, route2);
    printf("min intersection distance = %i\n", minDistance);
    free((void*)route1);
    free((void*)route2);
    fclose(file);
}