#include <unity.h>
#include <stdlib.h>
#include <stdio.h>
#include "sol.h"

struct Map one;
struct Map two;
struct Map three;
struct Map four;
struct Map five;

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
}

void tearDown() {}

void mapParsedCorrectly() {
    int expectedRow1[] = {0,1,0,0,1};
    int expectedRow2[] = {0,0,0,0,0};
    int expectedRow3[] = {1,1,1,1,1};
    int expectedRow4[] = {0,0,0,0,1};
    int expectedRow5[] = {0,0,0,1,1};
    TEST_ASSERT_EQUAL_INT_ARRAY(expectedRow1, one.asteroidLocations[0], 5);
    TEST_ASSERT_EQUAL_INT_ARRAY(expectedRow2, one.asteroidLocations[1], 5);
    TEST_ASSERT_EQUAL_INT_ARRAY(expectedRow3, one.asteroidLocations[2], 5);
    TEST_ASSERT_EQUAL_INT_ARRAY(expectedRow4, one.asteroidLocations[3], 5);
    TEST_ASSERT_EQUAL_INT_ARRAY(expectedRow5, one.asteroidLocations[4], 5);
}

void stationDetectedAt10() {
    struct Coordinate asteroidLocation = {.x=1, .y=0};
    TEST_ASSERT_EQUAL_INT(1, isAsteroid(one, asteroidLocation));
}

void noStationDetectedAt00() {
    struct Coordinate emptyLocation = {.x=1, .y=0};
    TEST_ASSERT_EQUAL_INT(0, isAsteroid(one, emptyLocation));
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
    struct Coordinate location = maxAsteroidsVisibleLocation(&sightings, one);
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
    struct Coordinate location = maxAsteroidsVisibleLocation(&sightings, one);
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
    struct Coordinate location = maxAsteroidsVisibleLocation(&sightings, one);
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
    struct Coordinate location = maxAsteroidsVisibleLocation(&sightings, one);
    TEST_ASSERT_EQUAL_INT(11, location.x);
    TEST_ASSERT_EQUAL_INT(13, location.y);
    TEST_ASSERT_EQUAL_INT(210, sightings);
 }


// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(maxSightingsShouldReturn8At34);
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
    RUN_TEST(maxSightingsShouldReturn33At58);
    RUN_TEST(maxSightingsShouldReturn35At12);
    RUN_TEST(maxSightingsShouldReturn41At63);
    RUN_TEST(maxSightingsShouldReturn210At1113);
    return UNITY_END();
}