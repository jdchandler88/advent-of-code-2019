#include <unity.h>
#include "password.h"

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
    TEST_ASSERT_EQUAL_INT(1, testPassword("111111"));
}

//-223450 does not meet these criteria (decreasing pair of digits 50).
void testPass2() {
    TEST_ASSERT_EQUAL_INT(0, testPassword("223450"));
}

//-123789 does not meet these criteria (no double).
void testPass3() {
    TEST_ASSERT_EQUAL_INT(0, testPassword("123789"));
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(testPass1);
    RUN_TEST(testPass2);
    RUN_TEST(testPass3);
    return UNITY_END();
}