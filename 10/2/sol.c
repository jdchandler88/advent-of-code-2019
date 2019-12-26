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
const float PI = M_PI;
const float PI_OVER_2 = PI/2;
const float ANGLE_ERROR = .001;

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
        free((void*)lines[i]);
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

float angleToLocation(struct Coordinate begin, struct Coordinate end) {
    int diffx = end.x - begin.x;
    int diffy = end.y - begin.y;
    return atan2(diffy, diffx);
}

float distanceToLocation(struct Coordinate begin, struct Coordinate end) {
    int dx = end.x - begin.x;
    int dy = end.y - begin.y;
    return sqrt(dx*dx + dy*dy);
}

static bool floatAlmostEquals(float a, float b) {
    return fabs(a-b) < ANGLE_ERROR;
}

bool isAsteroidObstructedFromLocation(struct Map map, struct Coordinate begin, struct Coordinate asteroid) {
    
    //calculate the angle between these two
    float angleBetweenStartAndEnd = angleToLocation(begin, asteroid);
    float distanceBetweenStartAndEnd = distanceToLocation(begin, asteroid);

    //let's try to be slightly clever about this. we know that given a direction, we can simply exclude 3 "quadrants" of candidates.
    //for every grid point (not being clever yet)
    for (int x=0; x<map.width; x++) {
        for (int y=0; y<map.height; y++) {
           struct Coordinate location = coordinate(x, y);
            //make sure we're not trying to examine the asteroid location. the asteroid cannot obstruct the view of itself
            if ( (location.x==asteroid.x && location.y==asteroid.y) || (location.x==begin.x && location.y==begin.y) ) {
                continue;
            }    
            //is it an asteroid?
            bool isAsteroidAtLocation = isAsteroid(map,  location);
            if (!isAsteroidAtLocation) {
                //no asteroid here. cannot obstruct view.
                continue;
            }
            // printf("checking (%i,%i)\n", x, y);
            //is it in between? (distance less than the distance between start/end)
            float distance = distanceToLocation(begin, location);
            if (distance > distanceBetweenStartAndEnd) {
                //it's farther away than the one we're trying to see. not obstructed
                continue;
            }

            //angle between begin and  grid point?
            float angle = angleToLocation(begin, location);
            float angleDifference = angleBetweenStartAndEnd-angle;
            if (fabs(angleDifference) < ANGLE_ERROR) {
                //there is an asteroid at a distance less than the one we want to see at the same angle. obstructed.
                return true;
            }
        }
    }
    //we didn't find an obstruction
    return false;
}

int countAsteriodsVisibleAtLocation(struct Map map, struct Coordinate location) {
    int count = 0;
    for (int x=0; x<map.width; x++) {
        for (int y=0; y<map.height; y++) {
            struct Coordinate examineLocation = coordinate(x, y);
            if (x==location.x && y==location.y) {
                //don't count if examine location is the same place we're at
                continue;
            }
            if (isAsteroid(map, examineLocation) && !isAsteroidObstructedFromLocation(map, location, examineLocation)) {
                count++;
            }
        }
    }
    return count;
}

struct Coordinate maxAsteroidsVisibleLocation(int* maxSightings, struct Map map) {
    *maxSightings = 0;
    struct Coordinate maxSightingsLocation;
    for (int x=0; x<map.width; x++) {
        for (int y=0; y<map.height; y++) {
            struct Coordinate coord = coordinate(x, y);
            //can only land on an asteroid. don't count empty spaces
            if (!isAsteroid(map, coord)) {
                continue;
            }
            int sightings = countAsteriodsVisibleAtLocation(map, coord);
            if (sightings > *maxSightings) {
                *maxSightings = sightings;
                maxSightingsLocation = coord;
            }
        }
    }
    return maxSightingsLocation;
}

struct Coordinate destroyNextAsteroid(struct Map map, float lastAngle) {

}