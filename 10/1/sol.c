#include "sol.h"
#include "utils.h"
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const char** lines;

static int numLines = 0;

static pthread_mutex_t mutex;


#define M_PI 3.14159265358979323846
const double PI = M_PI;
const double PI_OVER_2 = PI/2;
const double ANGLE_ERROR = .001;

static void lineCallback(const char* line) { 
    numLines++;
    if (numLines == 1) {
        lines = malloc(sizeof(const char*));
    } else {
        lines = realloc(lines, numLines*sizeof(const char*));
    }
    int lineLen = strlen(line);
    char* lineCopy = malloc(lineLen*sizeof(char));
    strcpy(lineCopy, line);
    lines[numLines-1] = lineCopy;
}

struct Coordinate coordinate(int x, int y) {
    struct Coordinate coord;
    coord.x = x;
    coord.y = y;
    return coord;
}

struct Map parseMap(FILE* file) {
    pthread_mutex_lock(&mutex);
    //read lines into memory
    readLines(file, lineCallback);

    //allocate storage for asteroid locations. these are square, for now
    struct Map map;
    map.width = numLines;
    map.height = numLines;
    map.asteroidLocations = malloc(numLines*sizeof(bool*));
    for (int i=0; i<numLines; i++) {
        map.asteroidLocations[i] = malloc(numLines*sizeof(bool));
    }

    //iterate over lines and populate map with information
    for (int y=0; y<numLines; y++) {
        const char* line = lines[y];
        int lineLen = strlen(line) - 1;
        for (int x=0; x<lineLen; x++) {
            const char point = line[x];
            if (point == '.') {
                map.asteroidLocations[x][y] = false;
            } else if (point == '#') {
                map.asteroidLocations[x][y] = true;
            }
        }
    }

    //free memory from lines
    for (int i=0; i<numLines; i++) {
        free(lines[i]);
    }
    free(lines);
    numLines = 0;
    pthread_mutex_unlock(&mutex);

    //return the parsed map
    return map;
}

bool isAsteroid(struct Map map, struct Coordinate coordinate) {
    return map.asteroidLocations[coordinate.x][coordinate.y];
}

double angleToLocation(struct Coordinate begin, struct Coordinate end) {
    int diffx = end.x - begin.x;
    int diffy = end.y - begin.y;
    return atan2(diffy, diffx);
}

bool isAsteroidObstructedFromLocation(struct Map map, struct Coordinate begin, struct Coordinate asteroid) {

}

int countAsteriodsVisibleAtLocation(struct Map map, struct Coordinate location) {

}

struct Coordinate maxAsteroidsVisibleLocation(int* maxSightings, struct Map map) {
    
}