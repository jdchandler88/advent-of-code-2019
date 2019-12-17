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

void queueSizeShouldBeZeroAfterCreation() {
    struct Queue* queue = createQueue();
    TEST_ASSERT_EQUAL_INT(0, sizeQueue(queue));
    destroyQueue(queue);
}

void queueSizeShouldBeOneAfterOneInsertion() {
    struct Queue* queue = createQueue();
    pushQueue(queue, 1);
    TEST_ASSERT_EQUAL_INT(1, sizeQueue(queue));
    destroyQueue(queue);
}

void queueValueShouldBeOneAfterOneAdd() {
    struct Queue* queue = createQueue();
    pushQueue(queue, 1);
    TEST_ASSERT_EQUAL_INT(1, popQueue(queue));
    destroyQueue(queue);
}

void queueShouldBehaveAppropriatelyDuringManyAdds() {
    int numTests = 1000;
    struct Queue* queue = createQueue();
    for (int i=0; i<numTests; i++) {
        pushQueue(queue, i);
        TEST_ASSERT_EQUAL_INT(i+1, sizeQueue(queue));
    }

    TEST_ASSERT_EQUAL_INT(numTests, sizeQueue(queue));

    for (int i=numTests-1; i>=0; i--) {
        int value = popQueue(queue);
        TEST_ASSERT_EQUAL_INT(i, value);
        TEST_ASSERT_EQUAL_INT(i, sizeQueue(queue));
    }
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

    RUN_TEST(queueSizeShouldBeZeroAfterCreation);
    RUN_TEST(queueSizeShouldBeOneAfterOneInsertion);
    RUN_TEST(queueValueShouldBeOneAfterOneAdd);
    RUN_TEST(queueShouldBehaveAppropriatelyDuringManyAdds);
    return UNITY_END();
}