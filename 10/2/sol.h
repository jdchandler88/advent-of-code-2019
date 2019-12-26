#include <stdbool.h>
#include <stdio.h>

typedef struct Coordinate {
  int x;
  int y;
} Coordinate;

typedef struct Map {
  int width;
  int height;
  bool** asteroidLocations;
} Map;

//constants
const float PI;
const float PI_OVER_2;
const float ANGLE_ERROR;

struct Coordinate coordinate(int x, int y);

struct Map parseMap(FILE* file);

bool isAsteroid(struct Map map, struct Coordinate coordinate);

float angleToLocation(struct Coordinate begin, struct Coordinate end);

float distanceToLocation(struct Coordinate begin, struct Coordinate end);

bool isAsteroidObstructedFromLocation(struct Map map, struct Coordinate begin, struct Coordinate asteroid);

int countAsteriodsVisibleAtLocation(struct Map map, struct Coordinate location);

struct Coordinate maxAsteroidsVisibleLocation(int* maxSightings, struct Map map);

struct Coordinate destroyNextAsteroid(struct Map map, float lastAngle);