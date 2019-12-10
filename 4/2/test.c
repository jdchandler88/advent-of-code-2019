#include <unity.h>
#include "sol.h"

int programCounter = 0;

void setUp() {
    programCounter = 0;
}

void tearDown() {}

/**
 * 'Acceptance criteria' used for tests
    -112233 meets these criteria because the digits never decrease and all repeated digits are exactly two digits long.
    -123444 no longer meets the criteria (the repeated 44 is part of a larger group of 444).
    -111122 meets the criteria (even though 1 is repeated more than twice, it still contains a double 22).
 * 
**/

//-112233 meets these criteria because the digits never decrease and all repeated digits are exactly two digits long.
void testPass1() {
    TEST_ASSERT_EQUAL_INT(1, testPassword(INT_MIN, INT_MAX, "112233"));
}

//-123444 no longer meets the criteria (the repeated 44 is part of a larger group of 444).
void testPass2() {
    TEST_ASSERT_EQUAL_INT(0, testPassword(INT_MIN, INT_MAX, "123444"));
}

//-111122 meets the criteria (even though 1 is repeated more than twice, it still contains a double 22).
void testPass3() {
    TEST_ASSERT_EQUAL_INT(1, testPassword(INT_MIN, INT_MAX, "111122"));
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(testPass1);
    RUN_TEST(testPass2);
    RUN_TEST(testPass3);
    return UNITY_END();
}