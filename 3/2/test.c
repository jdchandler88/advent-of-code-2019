#include <unity.h>
#include "wires.h"

int programCounter = 0;

void setUp() {
    programCounter = 0;
}

void tearDown() {}

/**
 * 'Acceptance criteria' used for tests

    -R8,U5,L5,D3
    U7,R6,D4,L4 = distance 6

    -R75,D30,R83,U83,L12,D49,R71,U7,L72
    U62,R66,U55,R34,D71,R55,D58,R83 = distance 159
    
    -R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51
    U98,R91,D20,R16,D67,R40,U7,R15,U6,R7 = distance 135
 * 
**/

/**
 * -R8,U5,L5,D3
    U7,R6,D4,L4 = distance 6
 **/ 
void testFirstWires() {
    const char* route1 = "R8,U5,L5,D3";
    const char* route2 = "U7,R6,D4,L4";
    TEST_ASSERT_EQUAL_INT(30, calculateMinIntersectionStepsFromRouteStrings(route1, route2));
}

/**
 * -R75,D30,R83,U83,L12,D49,R71,U7,L72
    U62,R66,U55,R34,D71,R55,D58,R83 = distance 159
 **/ 
void testSecondWires() {
    const char* route1 = "R75,D30,R83,U83,L12,D49,R71,U7,L72";
    const char* route2 = "U62,R66,U55,R34,D71,R55,D58,R83";
    TEST_ASSERT_EQUAL_INT(610, calculateMinIntersectionStepsFromRouteStrings(route1, route2));
}

/**
 * -R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51
    U98,R91,D20,R16,D67,R40,U7,R15,U6,R7 = distance 135
 **/ 
void testThirdWires() {
    const char* route1 = "R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51";
    const char* route2 = "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7";
    TEST_ASSERT_EQUAL_INT(410, calculateMinIntersectionStepsFromRouteStrings(route1, route2));
}

void testParseMovements() {
    int size;
    struct Movement** movements = parseMovements("U10,D20,L30,R40", &size);
    TEST_ASSERT_EQUAL_INT(UP, movements[0]->direction);
    TEST_ASSERT_EQUAL_INT(10, movements[0]->numMoves);
    TEST_ASSERT_EQUAL_INT(DOWN, movements[1]->direction);
    TEST_ASSERT_EQUAL_INT(20, movements[1]->numMoves);
    TEST_ASSERT_EQUAL_INT(LEFT, movements[2]->direction);
    TEST_ASSERT_EQUAL_INT(30, movements[2]->numMoves);
    TEST_ASSERT_EQUAL_INT(RIGHT, movements[3]->direction);
    TEST_ASSERT_EQUAL_INT(40, movements[3]->numMoves);
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(testFirstWires);
    RUN_TEST(testSecondWires);
    RUN_TEST(testThirdWires);
    return UNITY_END();
}