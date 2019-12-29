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
const float INITIAL_SEARCH_ANGLE = PI_OVER_2-ANGLE_ERROR;

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

static float conv = (180/PI);

typedef struct AsteroidInfo {
    struct Coordinate location;
    float angle;
    float distance;
    bool destroyed;
} AsteroidInfo;

static int sortAsteroid(const void* a, const void* b) {
    struct AsteroidInfo* info1 = (struct AsteroidInfo*)a;
    struct AsteroidInfo* info2 = (struct AsteroidInfo*)b;
    if (floatAlmostEquals(info1->angle, info2->angle)) {
        if (info1->distance < info2->distance) {
            return -1;  // a < b
        } else {
            return 1;   //a > b
        }
    } else if (info1->angle < info2->angle) {
        return -1;  //  a < b
    } else {
        return 1;   // a > b
    }
}

struct Coordinate destroyAsteroids(struct Map map, struct Coordinate location, DestructionCallback callback) {
    
    //create information about stations
    struct AsteroidInfo stations[map.width*map.height];
    int stationCount = 0;
    for (int x=0; x<map.width; x++) {
        for (int y=0; y<map.height; y++) {
            struct Coordinate asteroid = coordinate(x, y);
            //don't do anything if there is no asteroid or the current iteration points to the location of the laser station
            if (!isAsteroid(map, coordinate(x,y)) || ( location.x==asteroid.x && location.y==asteroid.y )) {
                continue;
            }
            struct AsteroidInfo stationInfo;
            memcpy(&stationInfo.location, &asteroid, sizeof(struct Coordinate));
            // stationInfo.location = asteroid;
            stationInfo.angle = wrapAngle02Pi(angleToLocation(location, asteroid) + PI);
            stationInfo.distance = distanceToLocation(location, asteroid);
            stationInfo.destroyed = false;
            stations[stationCount++] = stationInfo;
        }
    }

    //sort them based on angle and distance
    qsort(stations, stationCount, sizeof(struct AsteroidInfo), sortAsteroid);

    //find starting asteroid and use a cursor. this cursor will rotate around the array
    int stationIdx = 0;
    for (int i=0; i<stationCount; i++) {
        AsteroidInfo info = stations[i];
        if (info.angle > INITIAL_SEARCH_ANGLE) {
            stationIdx = i;
            break;
        }
    }

    //start looping through array destroying each subsequent element. if there are contiguous 
    //entries with the same angle, then those need to be skipped (until the next rotation)
    int destructionCount = 0;
    float lastAngles = INITIAL_SEARCH_ANGLE;
    while (destructionCount < stationCount) {
        for (int i=0; i<stationCount; i++) {
            int remaining = stationCount-destructionCount;
              AsteroidInfo info = stations[i + stationIdx];
            bool skipThisAsteroid = false;
            //is the station already destroyed? if so, then don't do anything.
            //is the angle the same as one that was previously destroyed? if so, then don't do anything
            if (info.destroyed || (floatAlmostEquals(info.angle, lastAngles) && remaining > 1)) {
                // continue;
                skipThisAsteroid = true;
            }

            if (!skipThisAsteroid) {
                //destroy asteroid
                info.destroyed = true;
                lastAngles = info.angle;
                stations[i+stationIdx] = info;
                destructionCount++;
                //notify listener
                callback(info.location);
            }
            

            //loop back to beginning of array if none was found by the end
            if ( (i + stationIdx) == stationCount - 1) {
                //reset cursor
                stationIdx = 0;
                lastAngles = stations[0].angle - ANGLE_ERROR;
                break;  //exit the current for loop so we can restart with updated initial cursor
            }

        }
    }
}

float wrapAngle02Pi(float angle) {
    float x = fmod(angle,2*PI);
    if (x < 0)
        x += 2*PI;
    return x;
}