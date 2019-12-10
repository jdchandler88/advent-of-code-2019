#include <unity.h>
#include "sol.h"

int programCounter = 0;

void setUp() {
    programCounter = 0;
}

void tearDown() {}

/**
 * 'Acceptance criteria' used for tests

    -111111 meets these criteria (double 11, never decreases).
    -223450 does not meet these criteria (decreasing pair of digits 50).
    -123789 does not meet these criteria (no double).

 * 
**/

//-111111 meets these criteria (double 11, never decreases).
void testPass1() {
    TEST_ASSERT_EQUAL_INT(1, testPassword(INT_MIN, INT_MAX, "111111"));
}

//-223450 does not meet these criteria (decreasing pair of digits 50).
void testPass2() {
    TEST_ASSERT_EQUAL_INT(0, testPassword(INT_MIN, INT_MAX, "223450"));
}

//-123789 does not meet these criteria (no double).
void testPass3() {
    TEST_ASSERT_EQUAL_INT(0, testPassword(INT_MIN, INT_MAX, "123789"));
}

//644444 should not meet criteria becuase the second digit decreased
void testPass4() {
    TEST_ASSERT_EQUAL_INT(0, testPassword(INT_MIN, INT_MAX, "644444"));
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(testPass1);
    RUN_TEST(testPass2);
    RUN_TEST(testPass3);
    RUN_TEST(testPass4);
    return UNITY_END();
}