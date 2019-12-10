#include <unity.h>
#include "sol.h"

int programCounter = 0;

void setUp() {
    programCounter = 0;
}

void tearDown() {}

/**
 * 'Acceptance criteria' used for tests
    1,0,0,0,99 becomes 2,0,0,0,99 (1 + 1 = 2).
    2,3,0,3,99 becomes 2,3,0,6,99 (3 * 2 = 6).
    2,4,4,5,99,0 becomes 2,4,4,5,99,9801 (99 * 99 = 9801).
    1,1,1,4,99,5,6,0,99 becomes 30,1,1,4,2,5,6,0,99.
 * 
**/

//1,0,0,0,99 becomes 2,0,0,0,99 (1 + 1 = 2).
void testFirstProgram() {
    int programSize = 5;
    int program[] = {1,0,0,0,99};
    int expected[] = {2,0,0,0,99};
    //execute program
    executeProgram(program, programSize);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected, program, programSize);
}

//2,3,0,3,99 becomes 2,3,0,6,99 (3 * 2 = 6).
void testSecondProgram() {
    int programSize = 5;
    int program[] = {2,3,0,3,99};
    int expected[] = {2,3,0,6,99};
    //execute program
    executeProgram(program, programSize);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected, program, programSize);
}

//2,4,4,5,99,0 becomes 2,4,4,5,99,9801 (99 * 99 = 9801).
void testThirdProgram() {
    int programSize = 6;
    int program[] = {2,4,4,5,99,0};
    int expected[] = {2,4,4,5,99,9801};
    //execute program
    executeProgram(program, programSize);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected, program, programSize);
}

//1,1,1,4,99,5,6,0,99 becomes 30,1,1,4,2,5,6,0,99
void testFourthProgram() {
    int programSize = 9;
    int program[] = {1,1,1,4,99,5,6,0,99};
    int expected[] = {30,1,1,4,2,5,6,0,99};
    //execute program
    executeProgram(program, programSize);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected, program, programSize);
}

/**
 * new requirements
 * 
 * 
    Parameter modes are stored in the same value as the instruction's opcode. 
    The opcode is a two-digit number based only on the ones and tens digit of 
    the value, that is, the opcode is the rightmost two digits of the first 
    value in an instruction. Parameter modes are single digits, one per parameter, 
    read right-to-left from the opcode: the first parameter's mode is in the hundreds 
    digit, the second parameter's mode is in the thousands digit, the third 
    parameter's mode is in the ten-thousands digit, and so on. Any missing modes 
    are 0.
  
    Opcode 3 takes a single integer as input and saves it to the position 
    given by its only parameter. For example, the instruction 3,50 would 
    take an input value and store it at address 50.

    Opcode 4 outputs the value of its only parameter. For example, the 
    instruction 4,50 would output the value at address 50.

    Programs that use these instructions will come with documentation that 
    explains what should be connected to the input and output. The program 
    3,0,4,0,99 outputs whatever it gets as input, then halts.

    !!!IN ORDER TO TEST THESE THINGS, WE WILL NEED TO ABSTRACT READING 
    INPUT AND WRITING OUTPUT!!!
**/

/**
 * ABCDE
    1002

DE - two-digit opcode,      02 == opcode 2
 C - mode of 1st parameter,  0 == position mode
 B - mode of 2nd parameter,  1 == immediate mode
 A - mode of 3rd parameter,  0 == position mode,
                                  omitted due to being a leading zero
 **/ 
void testParsingMixedModeInstruction() {
    const char* opcode = "1002";
    //parse the opcode into a struct and make assertions
    TEST_ASSERT_EQUAL_INT(0, 1);
}

/**
 * 1002,4,3,4,33
 * 
 * This instruction multiplies its first two parameters. 
 * The first parameter, 4 in position mode, works like it did before
 *  - its value is the value stored at address 4 (33). The second 
 * parameter, 3 in immediate mode, simply has value 3. The result of 
 * this operation, 33 * 3 = 99, is written according to the third 
 * parameter, 4 in position mode, which also works like it did before
 *  - 99 is written to address 4.
 **/ 
void testExecuteMixedModeInstruction() {
    int programSize = 5;
    int program[] = {1002,4,3,4,33};
    int expected[] = {1002,4,3,4,99};
    //execute program
    executeProgram(program, programSize);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected, program, programSize);
}

/**
 * Opcode 3 takes a single integer as input and saves it to the position 
    given by its only parameter. For example, the instruction 3,50 would 
    take an input value and store it at address 50.
**/ 
void testInput() {
    int programSize = 3;
    int program[] = {3,1,99};
    int expected[] = {30,1234,99};
    //execute program
    executeProgram(program, programSize);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected, program, programSize);
}

/**
 * Opcode 4 outputs the value of its only parameter. For example, the 
    instruction 4,50 would output the value at address 50.
**/
void testOutput() {
    int programSize = 3;
    int program[] = {4,0,99};
    //execute program
    //need to add 'ouptput writer' that will capture output to wherever we specify
    executeProgram(program, programSize);
    TEST_ASSERT_EQUAL_INT(0, 1);
}

/**
 * Opcode 3 takes a single integer as input and saves it to the position 
    given by its only parameter. For example, the instruction 3,50 would 
    take an input value and store it at address 50.
**/
void testSimpleIOProgram() {
    int programSize = 3;
    int program[] = {3,0,4,0,99};
    //execute program
    //need to add 'ouptput writer' that will capture output to wherever we specify
    executeProgram(program, programSize);
    TEST_ASSERT_EQUAL_INT(0, 1);
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(testFirstProgram);
    RUN_TEST(testSecondProgram);
    RUN_TEST(testThirdProgram);
    RUN_TEST(testFourthProgram);
    RUN_TEST(testParsingMixedModeInstruction);
    RUN_TEST(testExecuteMixedModeInstruction);
    RUN_TEST(testInput);
    RUN_TEST(testOutput);
    RUN_TEST(testSimpleIOProgram);
    return UNITY_END();
}