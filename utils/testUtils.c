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

void copyCopiesStringSmall() {
    const char* orig = "small";
    const char* copy = copyString(orig);
    TEST_ASSERT_EQUAL_STRING(orig, copyString(orig));
    free((void*)copy);
}

void copyCopiesStringLarge() {
    const char* orig = "abdefajsflkajsd;flkjasd;lfkjasd;flkjasd;flkjasd;flkjasdf;llkjasdf;lkjsdaf;lkjasdf;lkjasdfl;kjasdf;lkj498723984729837498237498ohdfjkhsdkjfhaslkdjfhsadfsdaflkasjdf";
    const char* copy = copyString(orig);
    TEST_ASSERT_EQUAL_STRING(orig, copyString(orig));
    free((void*)copy);
}

void parseParsesSpaces() {
    const char* string = "a b c d e";
    const char* expected[] = {"a", "b", "c", "d", "e"};
    int num;
    const char** actual = parseString(string, " ", &num);
    TEST_ASSERT_EQUAL_STRING_ARRAY(expected, actual, 5);
    TEST_ASSERT_EQUAL_INT(num, 5);
}

void parseParsesCommas() {
    const char* string = "a,b,c,d,e";
    const char* expected[] = {"a", "b", "c", "d", "e"};
    int num;
    const char** actual = parseString(string, ",", &num);
    TEST_ASSERT_EQUAL_STRING_ARRAY(expected, actual, 5);
    TEST_ASSERT_EQUAL_INT(num, 5);
}

void parseParsesSpacesAndCommas() {
    const char* string = "a, b, c,d e"; //space AND comma, comma, space
    const char* expected[] = {"a", "b", "c", "d", "e"};
    int num;
    const char** actual = parseString(string, " ,", &num);
    TEST_ASSERT_EQUAL_STRING_ARRAY(expected, actual, 5);
    TEST_ASSERT_EQUAL_INT(num, 5);
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(readLinesShouldReturnThreeLinesWithThreeLinesDotText);
    RUN_TEST(readLinesShouldReturnZeroLinesWithEmptyFile);
    RUN_TEST(copyCopiesStringSmall);
    RUN_TEST(copyCopiesStringLarge);
    RUN_TEST(parseParsesSpaces);
    RUN_TEST(parseParsesCommas);
    RUN_TEST(parseParsesSpacesAndCommas);
    return UNITY_END();
}