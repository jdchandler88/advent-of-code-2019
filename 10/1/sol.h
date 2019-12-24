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
const double PI;
const double PI_OVER_2;
const double ANGLE_ERROR;

struct Coordinate coordinate(x, y);

struct Map parseMap(FILE* file);

bool isAsteroid(struct Map map, struct Coordinate coordinate);

double angleToLocation(struct Coordinate begin, struct Coordinate end);

bool isAsteroidObstructedFromLocation(struct Map map, struct Coordinate begin, struct Coordinate asteroid);

int countAsteriodsVisibleAtLocation(struct Map map, struct Coordinate location);

struct Coordinate maxAsteroidsVisibleLocation(int* maxSightings, struct Map map);