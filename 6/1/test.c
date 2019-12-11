#include <unity.h>
#include "sol.h"

void setUp() {}

void tearDown() {}

/**
 * 
COM)B
B)C
C)D
D)E
E)F
B)G
G)H
D)I
E)J
J)K
K)L

should return 42
 **/ 
void testCountOrbitsShouldReturn42() {
    const char* mappings[] = {"COM)B", "B)C", "C)D", "D)E", "E)F", "B)G", "G)H", "D)I", "E)J", "J)K", "K)L"};
    TEST_ASSERT_EQUAL_INT(42, countOrbits(mappings, 11));
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(testCountOrbitsShouldReturn42);
    return UNITY_END();
}