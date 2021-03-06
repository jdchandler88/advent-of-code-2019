#include <unity.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "sol.h"

struct Map one;
struct Map two;
struct Map three;
struct Map four;
struct Map five;
struct Map six;

struct Queue* destroyedAsteroids;

struct Map getMapFromFile(const char* filename) {
    FILE* f = fopen(filename, "r");
    struct Map map = parseMap(f);
    fclose(f);
    return map;
}

void setUp() {
    one = getMapFromFile("map1.txt");
    two = getMapFromFile("map2.txt");
    three = getMapFromFile("map3.txt");
    four = getMapFromFile("map4.txt");
    five = getMapFromFile("map5.txt");
    six = getMapFromFile("map6.txt");
    destroyedAsteroids = queue_create();
}

void tearDown() {
    queue_destroy(destroyedAsteroids);
}

void mapParsedCorrectly() {
    //row1
    TEST_ASSERT_EQUAL_INT(false, isAsteroid(one, coordinate(0, 0)));
    TEST_ASSERT_EQUAL_INT(true, isAsteroid(one, coordinate(1, 0)));
    TEST_ASSERT_EQUAL_INT(false, isAsteroid(one, coordinate(2, 0)));
    TEST_ASSERT_EQUAL_INT(false, isAsteroid(one, coordinate(3, 0)));
    TEST_ASSERT_EQUAL_INT(true, isAsteroid(one, coordinate(4, 0)));
    //row2
    TEST_ASSERT_EQUAL_INT(false, isAsteroid(one, coordinate(0, 1)));
    TEST_ASSERT_EQUAL_INT(false, isAsteroid(one, coordinate(1, 1)));
    TEST_ASSERT_EQUAL_INT(false, isAsteroid(one, coordinate(2, 1)));
    TEST_ASSERT_EQUAL_INT(false, isAsteroid(one, coordinate(3, 1)));
    TEST_ASSERT_EQUAL_INT(false, isAsteroid(one, coordinate(4, 1)));
    //row3
    TEST_ASSERT_EQUAL_INT(true, isAsteroid(one, coordinate(0, 2)));
    TEST_ASSERT_EQUAL_INT(true, isAsteroid(one, coordinate(1, 2)));
    TEST_ASSERT_EQUAL_INT(true, isAsteroid(one, coordinate(2, 2)));
    TEST_ASSERT_EQUAL_INT(true, isAsteroid(one, coordinate(3, 2)));
    TEST_ASSERT_EQUAL_INT(true, isAsteroid(one, coordinate(4, 2)));
    //row4
    TEST_ASSERT_EQUAL_INT(false, isAsteroid(one, coordinate(0, 3)));
    TEST_ASSERT_EQUAL_INT(false, isAsteroid(one, coordinate(1, 3)));
    TEST_ASSERT_EQUAL_INT(false, isAsteroid(one, coordinate(2, 3)));
    TEST_ASSERT_EQUAL_INT(false, isAsteroid(one, coordinate(3, 3)));
    TEST_ASSERT_EQUAL_INT(true, isAsteroid(one, coordinate(4, 3)));
    //row5
    TEST_ASSERT_EQUAL_INT(false, isAsteroid(one, coordinate(0, 4)));
    TEST_ASSERT_EQUAL_INT(false, isAsteroid(one, coordinate(1, 4)));
    TEST_ASSERT_EQUAL_INT(false, isAsteroid(one, coordinate(2, 4)));
    TEST_ASSERT_EQUAL_INT(true, isAsteroid(one, coordinate(3, 4)));
    TEST_ASSERT_EQUAL_INT(true, isAsteroid(one, coordinate(4, 4)));
}

void originTo10ShouldReturn0Angle() {
    struct Coordinate origin = {.x=0, .y=0};
    TEST_ASSERT_FLOAT_WITHIN(.001, 0, angleToLocation(origin, coordinate(1,0)));
}

void originTo01ShouldReturn90Angle() {
    struct Coordinate origin = {.x=0, .y=0};
    TEST_ASSERT_FLOAT_WITHIN(.001, PI_OVER_2, angleToLocation(origin, coordinate(0,1)));
}

void originToN10ShouldReturnPiAngle() {
    struct Coordinate origin = {.x=0, .y=0};
    TEST_ASSERT_FLOAT_WITHIN(.001, PI, angleToLocation(origin, coordinate(-1,0)));
}

void originTo0N1ShouldReturnN90Angle() {
    struct Coordinate origin = {.x=0, .y=0};
    TEST_ASSERT_FLOAT_WITHIN(.001, -1*PI_OVER_2, angleToLocation(origin, coordinate(0,-1)));
}

void originTo11ShouldReturnPIOver4Angle() {
    struct Coordinate origin = {.x=0, .y=0};
    TEST_ASSERT_FLOAT_WITHIN(.001, PI/4, angleToLocation(origin, coordinate(1,1)));
}

//this shows that we need to rotate the result because x increases DOWNWARD and not UP
void angleFrom1113To1112ShouldBePiOverTwo() {
    struct Coordinate origin = {.x=11, .y=13};
    float angle = angleToLocation(origin, coordinate(11,12)) + PI;
    angle = wrapAngle02Pi(angle);
    TEST_ASSERT_FLOAT_WITHIN(.001, PI/2, angle);
}

void oritinTo10ShouldReturn1Distance() {
    TEST_ASSERT_FLOAT_WITHIN(.001, 1, distanceToLocation(coordinate(0,0), coordinate(1,0)));
}

void originTo01ShouldReturn1Distance() {
    TEST_ASSERT_FLOAT_WITHIN(.001, 1, distanceToLocation(coordinate(0,0), coordinate(0,1)));
}

void originToN10ShouldReturn1Distance() {
    TEST_ASSERT_FLOAT_WITHIN(.001, 1, distanceToLocation(coordinate(0,0), coordinate(-1,0)));
}

void originTo0N1ShouldReturn1Distance() {
    TEST_ASSERT_FLOAT_WITHIN(.001, 1, distanceToLocation(coordinate(0,0), coordinate(0,-1)));
}

void originTo11ShouldReturnSqrt2Distance() {
    TEST_ASSERT_FLOAT_WITHIN(.001, sqrt(2), distanceToLocation(coordinate(0,0), coordinate(1,1)));
}

/**
 * need to make sure obstruction detection is working
 *  at (3,4), cannot detect (1,0)
 **/ 
//
void obstructedShouldReturnTrueFor10At34() {
    struct Coordinate viewpoint = {.x=3, .y=4};
    struct Coordinate asteroidLocation = {.x=1, .y=0};
    TEST_ASSERT_EQUAL_INT(1, isAsteroidObstructedFromLocation(one, viewpoint, asteroidLocation));
}

/**
 * Test each location using the acceptance criteria
 * 
 *  .#..#
    .....
    #####
    ....#
    ...##
    
    .7..7
    .....
    67775
    ....7
    ...87
 **/

void sightingsShouldReturn7At10() {
    struct Coordinate asteroidLocation = {.x=1, .y=0};
    TEST_ASSERT_EQUAL_INT(7, countAsteriodsVisibleAtLocation(one, asteroidLocation));
}
void sightingsShouldReturn7At40() {
    struct Coordinate asteroidLocation = {.x=4, .y=0};
    TEST_ASSERT_EQUAL_INT(7, countAsteriodsVisibleAtLocation(one, asteroidLocation));
}
void sightingsShouldReturn6At02() {
    struct Coordinate asteroidLocation = {.x=0, .y=2};
    TEST_ASSERT_EQUAL_INT(6, countAsteriodsVisibleAtLocation(one, asteroidLocation));
}
void sightingsShouldReturn7At12() {
    struct Coordinate asteroidLocation = {.x=1, .y=2};
    TEST_ASSERT_EQUAL_INT(7, countAsteriodsVisibleAtLocation(one, asteroidLocation));
}
void sightingsShouldReturn7At22() {
    struct Coordinate asteroidLocation = {.x=2, .y=2};
    TEST_ASSERT_EQUAL_INT(7, countAsteriodsVisibleAtLocation(one, asteroidLocation));
}
void sightingsShouldReturn7At32() {
    struct Coordinate asteroidLocation = {.x=3, .y=2};
    TEST_ASSERT_EQUAL_INT(7, countAsteriodsVisibleAtLocation(one, asteroidLocation));
}
void sightingsShouldReturn5At42() {
    struct Coordinate asteroidLocation = {.x=4, .y=2};
    TEST_ASSERT_EQUAL_INT(5, countAsteriodsVisibleAtLocation(one, asteroidLocation));
}
void sightingsShouldReturn7At43() {
    struct Coordinate asteroidLocation = {.x=4, .y=3};
    TEST_ASSERT_EQUAL_INT(7, countAsteriodsVisibleAtLocation(one, asteroidLocation));
}
void sightingsShouldReturn8At34() {
    struct Coordinate asteroidLocation = {.x=3, .y=4};
    TEST_ASSERT_EQUAL_INT(8, countAsteriodsVisibleAtLocation(one, asteroidLocation));
}
void sightingsShouldReturn7At44() {
    struct Coordinate asteroidLocation = {.x=4, .y=4};
    TEST_ASSERT_EQUAL_INT(7, countAsteriodsVisibleAtLocation(one, asteroidLocation));
}

/**
 * Overarching system test for code
 * 
    .#..#
    .....
    #####
    ....#
    ...##
 **/
void maxSightingsShouldReturn8At34() {
    int sightings;
    struct Coordinate location = maxAsteroidsVisibleLocation(&sightings, one);
    TEST_ASSERT_EQUAL_INT(3, location.x);
    TEST_ASSERT_EQUAL_INT(4, location.y);
    TEST_ASSERT_EQUAL_INT(8, sightings);
}

/**
 * Best is 5,8 with 33 other asteroids detected:

    ......#.#.
    #..#.#....
    ..#######.
    .#.#.###..
    .#..#.....
    ..#....#.#
    #..#....#.
    .##.#..###
    ##...#..#.
    .#....####

 **/ 
void maxSightingsShouldReturn33At58() {
    int sightings;
    struct Coordinate location = maxAsteroidsVisibleLocation(&sightings, two);
    TEST_ASSERT_EQUAL_INT(5, location.x);
    TEST_ASSERT_EQUAL_INT(8, location.y);
    TEST_ASSERT_EQUAL_INT(33, sightings);
}

/**
 * Best is 1,2 with 35 other asteroids detected:

    #.#...#.#.
    .###....#.
    .#....#...
    ##.#.#.#.#
    ....#.#.#.
    .##..###.#
    ..#...##..
    ..##....##
    ......#...
    .####.###.
 **/ 
void maxSightingsShouldReturn35At12() {
    int sightings;
    struct Coordinate location = maxAsteroidsVisibleLocation(&sightings, three);
    TEST_ASSERT_EQUAL_INT(1, location.x);
    TEST_ASSERT_EQUAL_INT(2, location.y);
    TEST_ASSERT_EQUAL_INT(35, sightings);
}

/**
 * Best is 6,3 with 41 other asteroids detected:

    .#..#..###
    ####.###.#
    ....###.#.
    ..###.##.#
    ##.##.#.#.
    ....###..#
    ..#.#..#.#
    #..#.#.###
    .##...##.#
    .....#.#..
 **/ 
void maxSightingsShouldReturn41At63() {
    int sightings;
    struct Coordinate location = maxAsteroidsVisibleLocation(&sightings, four);
    TEST_ASSERT_EQUAL_INT(6, location.x);
    TEST_ASSERT_EQUAL_INT(3, location.y);
    TEST_ASSERT_EQUAL_INT(41, sightings);
}

/**
 * 
 * Best is 11,13 with 210 other asteroids detected:

    .#..##.###...#######
    ##.############..##.
    .#.######.########.#
    .###.#######.####.#.
    #####.##.#.##.###.##
    ..#####..#.#########
    ####################
    #.####....###.#.#.##
    ##.#################
    #####.##.###..####..
    ..######..##.#######
    ####.##.####...##..#
    .#####..#.######.###
    ##...#.##########...
    #.##########.#######
    .####.#.###.###.#.##
    ....##.##.###..#####
    .#.#.###########.###
    #.#.#.#####.####.###
    ###.##.####.##.#..##
  **/
 void maxSightingsShouldReturn210At1113() {
    int sightings;
    struct Coordinate location = maxAsteroidsVisibleLocation(&sightings, five);
    TEST_ASSERT_EQUAL_INT(11, location.x);
    TEST_ASSERT_EQUAL_INT(13, location.y);
    TEST_ASSERT_EQUAL_INT(210, sightings);
 }

 /**Best is 11,13 with 210 other asteroids detected:
  * 
    The 1st asteroid to be vaporized is at 11,12.
    The 2nd asteroid to be vaporized is at 12,1.
    The 3rd asteroid to be vaporized is at 12,2.
    The 10th asteroid to be vaporized is at 12,8.
    The 20th asteroid to be vaporized is at 16,0.
    The 50th asteroid to be vaporized is at 16,9.
    The 100th asteroid to be vaporized is at 10,16.
    The 199th asteroid to be vaporized is at 9,6.
    The 200th asteroid to be vaporized is at 8,2.
    The 201st asteroid to be vaporized is at 10,9.
    The 299th and final asteroid to be vaporized is at 11,1.
  **/ 

void assertCoordinateEquals(struct Coordinate expected, struct Coordinate actual) {
    TEST_ASSERT_EQUAL_INT(expected.x, actual.x);
    TEST_ASSERT_EQUAL_INT(expected.y, actual.y);
}

void destructionCallback(struct Coordinate location) {
    //copy coordinate
    struct Coordinate* coord = malloc(sizeof(struct Coordinate));
    memcpy(coord, &location, sizeof(struct Coordinate));
    //creat equeue element for storage
    struct QueueElement el = {VOIDP, {.anyPointer=coord}};
    queue_enqueue(destroyedAsteroids, el);
}

/**
 * 
FIRST NINE
.#....###24...#..
##...##.13#67..9#
##...#...5.8####.
..#.....X...###..
..#.#.....#....##


SECOND NINE
.#....###.....#..
##...##...#.....#
##...#......1234.
..#.....X...5##..
..#.9.....8....76


THIRD NINE
.8....###.....#..
56...9#...#.....#
34...7...........
..2.....X....##..
..1..............

LAST NINE
......234.....6..
......1...5.....7
.................
........X....89..
.................
 **/ 
void testDestructionFunctionsProperlySmallMap() {
    struct Coordinate location = {.x=8, .y=3};
    struct Coordinate destructionLocations[] = {
        coordinate(8,1), coordinate(9,0), coordinate(9,1), coordinate(10,0), coordinate(9,2), coordinate(11,1), coordinate(12, 1), coordinate(11, 2), coordinate(15,1),
        coordinate(12,2), coordinate(13,2), coordinate(14,2), coordinate(15,2), coordinate(12,3), coordinate(16, 4), coordinate(15,4), coordinate(10,4), coordinate(4,4),
        coordinate(2,4), coordinate(2,3), coordinate(0,2), coordinate(1,2), coordinate(0,1), coordinate(1,1), coordinate(5,2), coordinate(1,0), coordinate(5,1),
        coordinate(6,1), coordinate(6,0), coordinate(7,0), coordinate(8,0), coordinate(10,1), coordinate(14, 0), coordinate(16,1), coordinate(13,3), coordinate(14,3)
    };
    destroyAsteroids(six, location, destructionCallback);
    for (int i=0; i<36; i++) {
        struct Coordinate destroyedAsteroid = *((struct Coordinate*)queue_dequeue(destroyedAsteroids).element.anyPointer);
        assertCoordinateEquals(destructionLocations[i], destroyedAsteroid);
    }
}

void testDestructionFunctionsPropertlyLargeMap() {

    struct Coordinate location = {.x=11, .y=13};
    float lastAngle = PI_OVER_2 - .001;
    destroyAsteroids(five, location, destructionCallback);
    for (int i=1; i<300; i++) {
        struct Coordinate destroyedAsteroid = *((struct Coordinate*)queue_dequeue(destroyedAsteroids).element.anyPointer);
        if (i==1) {
            assertCoordinateEquals(coordinate(11, 12), destroyedAsteroid);
        }
        if (i==2) {
            assertCoordinateEquals(coordinate(12,1), destroyedAsteroid);
        }
        if (i==3) {
            assertCoordinateEquals(coordinate(12, 2), destroyedAsteroid);
        }
        if (i==10) {
            assertCoordinateEquals(coordinate(12, 8), destroyedAsteroid);
        }
        if (i==20) {
            assertCoordinateEquals(coordinate(16, 0), destroyedAsteroid);
        }
        if (i==50) {
            assertCoordinateEquals(coordinate(16, 9), destroyedAsteroid);
        }
        if (i==100) {
            assertCoordinateEquals(coordinate(10, 16), destroyedAsteroid);
        }
        if (i==199) {
            assertCoordinateEquals(coordinate(9, 6), destroyedAsteroid);
        }
        if (i==200) {
            assertCoordinateEquals(coordinate(8, 2), destroyedAsteroid);
        }
        if (i==201) {
            assertCoordinateEquals(coordinate(10, 9), destroyedAsteroid);
        }
        if (i==299) {
            assertCoordinateEquals(coordinate(11, 1), destroyedAsteroid);
        }
    }
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(mapParsedCorrectly);
    RUN_TEST(originTo10ShouldReturn0Angle);
    RUN_TEST(originTo01ShouldReturn90Angle);
    RUN_TEST(originToN10ShouldReturnPiAngle);
    RUN_TEST(originTo0N1ShouldReturnN90Angle);
    RUN_TEST(originTo11ShouldReturnPIOver4Angle);
    RUN_TEST(angleFrom1113To1112ShouldBePiOverTwo);
    RUN_TEST(originToN10ShouldReturn1Distance);
    RUN_TEST(originTo01ShouldReturn1Distance);
    RUN_TEST(originToN10ShouldReturn1Distance);
    RUN_TEST(originTo0N1ShouldReturn1Distance);
    RUN_TEST(originTo11ShouldReturnSqrt2Distance);
    RUN_TEST(obstructedShouldReturnTrueFor10At34);
    RUN_TEST(sightingsShouldReturn7At10);
    RUN_TEST(sightingsShouldReturn7At40);
    RUN_TEST(sightingsShouldReturn6At02);
    RUN_TEST(sightingsShouldReturn7At12);
    RUN_TEST(sightingsShouldReturn7At22);
    RUN_TEST(sightingsShouldReturn7At32);
    RUN_TEST(sightingsShouldReturn5At42);
    RUN_TEST(sightingsShouldReturn7At43);
    RUN_TEST(sightingsShouldReturn8At34);
    RUN_TEST(sightingsShouldReturn7At44);
    RUN_TEST(maxSightingsShouldReturn8At34);
    RUN_TEST(maxSightingsShouldReturn33At58);
    RUN_TEST(maxSightingsShouldReturn35At12);
    RUN_TEST(maxSightingsShouldReturn41At63);
    RUN_TEST(maxSightingsShouldReturn210At1113);
    RUN_TEST(testDestructionFunctionsProperlySmallMap);
    RUN_TEST(testDestructionFunctionsPropertlyLargeMap);
    return UNITY_END();
}