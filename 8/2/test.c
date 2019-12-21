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

void leastZerosShouldBeInLayer0() {
    struct Image* image = parseImage(3, 2, "103050789012");
    TEST_ASSERT_EQUAL_INT(1, findLayerWithLeastDigitCount(image, 0));
}

/**
 * For example, given an image 2 pixels wide and 2 pixels tall, the image data 0222112222120000 corresponds to the following image layers:

Layer 1: 02
         22

Layer 2: 11
         22

Layer 3: 22
         12

Layer 4: 00
         00

Then, the full image can be found by determining the top visible pixel in each position:

    The top-left pixel is black because the top layer is 0.
    The top-right pixel is white because the top layer is 2 (transparent), but the second layer is 1.
    The bottom-left pixel is white because the top two layers are 2, but the third layer is 1.
    The bottom-right pixel is black because the only visible pixel in that position is 0 (from layer 4).

So, the final image looks like this:

01
10

 **/ 
void pixelsShouldRenderProperly() {
    struct Image* image = parseImage(2, 2, "0222112222120000");
    TEST_ASSERT_EQUAL_INT(0, composite(image)->pixels[0][0]);
    TEST_ASSERT_EQUAL_INT(1, composite(image)->pixels[0][1]);
    TEST_ASSERT_EQUAL_INT(1, composite(image)->pixels[1][0]);
    TEST_ASSERT_EQUAL_INT(0, composite(image)->pixels[1][1]);
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(imageShouldHaveTwoLayersAndAppropriateData);
    RUN_TEST(sumOf0InLayerShouldBeThreeAndOne);
    RUN_TEST(leastZerosShouldBeInLayer0);
    RUN_TEST(pixelsShouldRenderProperly);
    return UNITY_END();
}