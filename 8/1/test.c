#include <unity.h>
#include "sol.h"

void setUp() {}

void tearDown() {}

/**
 * 'Acceptance criteria' used for tests
 * 
    For example, given an image 3 pixels wide and 2 pixels tall, the image data 123456789012 corresponds to the following image layers:

Layer 1: 123
         456

Layer 2: 789
         012

 * 
**/

void imageShouldHaveTwoLayersAndAppropriateData() {
    struct Image* image = parseImage(3, 2, "123456789012");
    //number of layers
    TEST_ASSERT_EQUAL_INT(2, image->numLayers);
    //data in each layer
    TEST_ASSERT_EQUAL_STRING("123", image->layers[0]->pixels[0]);
    TEST_ASSERT_EQUAL_STRING("456", image->layers[0]->pixels[1]);
    TEST_ASSERT_EQUAL_STRING("789", image->layers[1]->pixels[0]);
    TEST_ASSERT_EQUAL_STRING("012", image->layers[1]->pixels[1]);
}

void sumOf0InLayerShouldBeThreeAndOne() {
    struct Image* image = parseImage(3, 2, "103050789012");
    TEST_ASSERT_EQUAL_INT(3, sumDigitInLayer(0, image->layers[0]));
    TEST_ASSERT_EQUAL_INT(1, sumDigitInLayer(0, image->layers[1]));
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(imageShouldHaveTwoLayersAndAppropriateData);
    RUN_TEST(sumOf0InLayerShouldBeThreeAndOne);
    return UNITY_END();
}