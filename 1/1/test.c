#include <unity.h>
#include <fuelCalculator.h>

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

}

void massOf14ShouldReturn2() {

}

void massOf1969Shouldreturn654() {

}

void massOf100756ShouldReturn33583() {

}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(testFunction);
    return UNITY_END();
}