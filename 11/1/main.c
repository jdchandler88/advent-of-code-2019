#include "sol.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count = 0;

/**
 * The Elves are placing bets on which will be the 200th asteroid to be vaporized. 
 * Win the bet by determining which asteroid that will be; what do you get if you 
 * multiply its X coordinate by 100 and then add its Y coordinate? (For example, 8,2 becomes 802.)
 **/ 
void destructionCallback(struct Coordinate location) {
    count++;
    if (count == 200) {
        printf("200th asteroid located at (%i,%i). Answer=%i\n", location.x, location.y, ((location.x*100) + location.y));
    }
}

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
    printf("destroying all asteroids at location.\n");
    destroyAsteroids(map, location, destructionCallback);
}