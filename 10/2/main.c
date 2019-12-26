#include "sol.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv) {
    if (argc != 2) {
        printf("gimme an input file, number of amplifiers we're decoding, feedbackMode, inputOffset. For example './exe input.txt 5 1 0'\n");
        printf("\tinputFile should be a string pointing to a file\n");
        exit(0);
    }
    FILE* f = fopen(argv[1], "r");
    struct Map map = parseMap(f);
    int sightings = 0;
    struct Coordinate location = maxAsteroidsVisibleLocation(&sightings, map);
    printf("max visible asteroids are %i from location (%i,%i)\n", sightings, location.x, location.y);
}