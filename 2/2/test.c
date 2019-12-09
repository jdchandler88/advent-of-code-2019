#include <unity.h>
#include "intcode.h"

int programCounter = 0;

void setUp() {
    programCounter = 0;
}

void tearDown() {}

/**
 * 'Acceptance criteria' used for tests
    1,0,0,0,99 becomes 2,0,0,0,99 (1 + 1 = 2).
    2,3,0,3,99 becomes 2,3,0,6,99 (3 * 2 = 6).
    2,4,4,5,99,0 becomes 2,4,4,5,99,9801 (99 * 99 = 9801).
    1,1,1,4,99,5,6,0,99 becomes 30,1,1,4,2,5,6,0,99.
 * 
**/

//1,0,0,0,99 becomes 2,0,0,0,99 (1 + 1 = 2).
void testFirstProgram() {
    int programSize = 5;
    int program[] = {1,0,0,0,99};
    int expected[] = {2,0,0,0,99};
    //execute program
    executeProgram(program, programSize);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected, program, programSize);
}

//2,3,0,3,99 becomes 2,3,0,6,99 (3 * 2 = 6).
void testSecondProgram() {
    int programSize = 5;
    int program[] = {2,3,0,3,99};
    int expected[] = {2,3,0,6,99};
    //execute program
    executeProgram(program, programSize);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected, program, programSize);
}

//2,4,4,5,99,0 becomes 2,4,4,5,99,9801 (99 * 99 = 9801).
void testThirdProgram() {
    int programSize = 6;
    int program[] = {2,4,4,5,99,0};
    int expected[] = {2,4,4,5,99,9801};
    //execute program
    executeProgram(program, programSize);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected, program, programSize);
}

//1,1,1,4,99,5,6,0,99 becomes 30,1,1,4,2,5,6,0,99
void testFourthProgram() {
    int programSize = 9;
    int program[] = {1,1,1,4,99,5,6,0,99};
    int expected[] = {30,1,1,4,2,5,6,0,99};
    //execute program
    executeProgram(program, programSize);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected, program, programSize);
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(testFirstProgram);
    RUN_TEST(testSecondProgram);
    RUN_TEST(testThirdProgram);
    RUN_TEST(testFourthProgram);
    return UNITY_END();
}