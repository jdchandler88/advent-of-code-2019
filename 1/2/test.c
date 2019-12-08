#include <unity.h>
#include "fuelCalculator.h"

void setUp() {}

void tearDown() {}

/**
 * 'Acceptance criteria' used for tests
 *  
    -A module of mass 14 requires 2 fuel. This fuel requires no 
    further fuel (2 divided by 3 and rounded down is 0, which 
    would call for a negative fuel), so the total fuel required 
    is still just 2.
    
    -At first, a module of mass 1969 requires 654 fuel. Then, this 
    fuel requires 216 more fuel (654 / 3 - 2). 216 then requires 70 
    more fuel, which requires 21 fuel, which requires 5 fuel, which r
    equires no further fuel. So, the total fuel required for a module 
    of mass 1969 is 654 + 216 + 70 + 21 + 5 = 966.

    -The fuel required by a module of mass 100756 and its fuel is: 
    33583 + 11192 + 3728 + 1240 + 411 + 135 + 43 + 12 + 2 = 50346.

 * 
**/

void massOf14ShouldReturn2() {
    TEST_ASSERT_EQUAL_INT (2, calculateFuel(14));
}

void massOf1969Shouldreturn966() {
    TEST_ASSERT_EQUAL_INT (966, calculateFuel(1969));
}

void massOf100756ShouldReturn50346() {
    TEST_ASSERT_EQUAL_INT (50346, calculateFuel(100756));
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(massOf14ShouldReturn2);
    RUN_TEST(massOf1969Shouldreturn966);
    RUN_TEST(massOf100756ShouldReturn50346);
    return UNITY_END();
}