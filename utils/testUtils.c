#include <unity.h>
#include <stdlib.h>
#include "utils.h"

int numLines = 0;
const char* curLine = NULL;

void setUp() {
    numLines = 0;
    curLine = NULL;
}

void tearDown() {}

void lineRead(const char* line) {
    curLine = line;
    numLines++;
}

void readLinesShouldReturnThreeLinesWithThreeLinesDotText() {
    FILE* threeLinesFile = fopen("./testData/threeLines.txt", "r");
    readLines(threeLinesFile, lineRead);
    TEST_ASSERT_EQUAL_INT(3, numLines);
    fclose(threeLinesFile);
}

void readLinesShouldReturnZeroLinesWithEmptyFile() {
    FILE* threeLinesFile = fopen("./testData/emptyFile.txt", "r");
    readLines(threeLinesFile, lineRead);
    TEST_ASSERT_EQUAL_INT(0, numLines);
    fclose(threeLinesFile);
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(readLinesShouldReturnThreeLinesWithThreeLinesDotText);
    RUN_TEST(readLinesShouldReturnZeroLinesWithEmptyFile);
    return UNITY_END();
}