#include <unity.h>
#include "fuelCalculator.h"

void setUp() {}

void tearDown() {}

/**
 * 'Acceptance criteria' used for tests
 * 
    For a mass of 12, divide by 3 and round down to get 4, then subtract 2 to get 2.
    For a mass of 14, dividing by 3 and rounding down still yields 4, so the fuel required is also 2.
    For a mass of 1969, the fuel required is 654.
    For a mass of 100756, the fuel required is 33583.
 * 
**/

void massOf12ShouldReturn2() {
    TEST_ASSERT_EQUAL_INT (2, calculateFuel(12));
}

void massOf14ShouldReturn2() {
    TEST_ASSERT_EQUAL_INT (2, calculateFuel(14));
}

void massOf1969Shouldreturn654() {
    TEST_ASSERT_EQUAL_INT (654, calculateFuel(1969));
}

void massOf100756ShouldReturn33583() {
    TEST_ASSERT_EQUAL_INT (33583, calculateFuel(100756));
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(massOf14ShouldReturn2);
    RUN_TEST(massOf1969Shouldreturn654);
    RUN_TEST(massOf100756ShouldReturn33583);
    return UNITY_END();
}