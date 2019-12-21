#include <unity.h>
#include <utils.h>
#include <stdbool.h>
#include <stdlib.h>
#include "sol.h"

int programCounter = 0;

const char* input;

const char** scrollingInput;

int currScrollingInput = 0;

int output;

struct InputReader* standardReader;

struct InputReader* chainReader;

struct InputReader* queueReader;

struct OutputWriter* standardWriter;

struct OutputWriter* chainWriter;

struct OutputWriter* queueWriter;

struct ProgramContext* context;

//it's up to each test to set the value of input to be returned by the reader
const char* readInput(void* arg) {
    return input;
}

//this method will scroll through a list of input values
const char* readScrollingInput(void* arg) {
    const char* input = scrollingInput[currScrollingInput];
    currScrollingInput++;
    return input;
}

void writeOutput(int programOutput, void* arg) {
    output = programOutput;
}

void writeScrollingOutput(int programOutput, void* arg) {
}

struct Queue* inputQueue;

struct Queue* outputQueue;

char outputBuffer[100];

const char* readQueue_test(void* arg) {
    struct Queue* queue = (struct Queue*)arg;
    sprintf(outputBuffer, "%d", popQueue(queue));
    return outputBuffer;
}

void writeQueue_test(int output, void* arg) {
    struct Queue* queue = (struct Queue*)arg;
    return pushQueue(queue, output);
}

void setUp() {
    programCounter = 0;
    input = NULL;
    scrollingInput = NULL;
    currScrollingInput = 0;

    context = malloc(sizeof(ProgramContext));
    standardReader = malloc(sizeof(InputReader));
    standardReader->reader = readInput;
    standardReader->readerContext = NULL;
    chainReader = malloc(sizeof(InputReader));
    chainReader->reader = readScrollingInput;
    chainReader->readerContext = NULL;
    queueReader = malloc(sizeof(InputReader));
    queueReader->reader = readQueue_test;
    queueReader->readerContext = inputQueue;
    standardWriter = malloc(sizeof(OutputWriter));
    standardWriter->writer = writeOutput;
    standardWriter->writerContext = NULL;
    chainWriter = malloc(sizeof(OutputWriter));
    chainWriter->writer = writeScrollingOutput;
    chainWriter->writerContext = NULL;
    queueWriter = malloc(sizeof(OutputWriter));
    queueWriter->writer = writeQueue_test;
    queueWriter->writerContext = outputQueue;

    inputQueue = createQueue();
    outputQueue = createQueue();
}

void tearDown() {
    free(context);
    free(standardReader);
    free(chainReader);
    free(queueReader);
    free(standardWriter);
    free(chainWriter);
    free(queueWriter);

    destroyQueue(inputQueue);
    destroyQueue(outputQueue);
}

struct ProgramContext* initializeContext(int* program, int programSize, InputReader* standardReader, OutputWriter* standardWriter) {
    context->programCounter = 0;
    context->program = program;
    context->programLength = programSize;
    context->reader = standardReader;
    context->writer = standardWriter;
}

void testParsingMixedModeInstruction() {
    int instructionInt = 1002;
    //parse the opcode into a struct
    struct Instruction* instruction = parseInstruction(instructionInt);
    //make the assertions
    TEST_ASSERT_EQUAL_INT(2, instruction->opcode);
    TEST_ASSERT_EQUAL_INT(0, instruction->parameter->mode);
    TEST_ASSERT_EQUAL_INT(1, instruction->parameter->next->mode);
    TEST_ASSERT_EQUAL_INT(0, instruction->parameter->next->next->mode);
}

void testParsingAddMixedModeInstruction() {
    int instructionInt = 10101;
    //parse the opcode into a struct
    struct Instruction* instruction = parseInstruction(instructionInt);
    //make the assertions
    TEST_ASSERT_EQUAL_INT(1, instruction->opcode);
    TEST_ASSERT_EQUAL_INT(1, instruction->parameter->mode);
    TEST_ASSERT_EQUAL_INT(0, instruction->parameter->next->mode);
    TEST_ASSERT_EQUAL_INT(1, instruction->parameter->next->next->mode);
}

void testParsingInputInstruction() {
    int instructionInt = 03;
    //parse the opcode into a struct
    struct Instruction* instruction = parseInstruction(instructionInt);
    //make the assertions
    TEST_ASSERT_EQUAL_INT(3, instruction->opcode);
    TEST_ASSERT_EQUAL_INT(0, instruction->parameter->mode);
}

void testParsingOutputInstruction() {
    int instructionInt = 04;
    //parse the opcode into a struct
    struct Instruction* instruction = parseInstruction(instructionInt);
    //make the assertions
    TEST_ASSERT_EQUAL_INT(4, instruction->opcode);
    TEST_ASSERT_EQUAL_INT(0, instruction->parameter->mode);
}

void testParsingHaltInstruction() {
    int instructionInt = 99;
    //parse the opcode into a struct
    struct Instruction* instruction = parseInstruction(instructionInt);
    //make the assertions
    TEST_ASSERT_EQUAL_INT(99, instruction->opcode);
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
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT_ARRAY(expected, program, programSize);
}

/**
 * Opcode 3 takes a single integer as input and saves it to the position 
    given by its only parameter. For example, the instruction 3,50 would 
    take an input value and store it at address 50.
**/ 
void testInput() {
    printf("ass\n");
    int programSize = 3;
    int program[] = {3,1,99};
    int expected[] = {3,1234,99};
    //test input
    input = "1234";
    //execute program
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
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
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT(4, output);
}

/**
 * Opcode 3 takes a single integer as input and saves it to the position 
    given by its only parameter. For example, the instruction 3,50 would 
    take an input value and store it at address 50.
**/
void testSimpleIOProgram() {
    int programSize = 5;
    int program[] = {3,0,4,0,99};
    int expected[] = {4321,0,4,0,99};
    input = "4321";
    //execute program
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT(4321, output);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected, program, programSize);
}

//3,9,8,9,10,9,4,9,99,-1,8 - Using position mode, consider whether the input is equal to 8; output 1 (if it is) or 0 (if it is not).
void testEqualTo8IndirectWithEqualInput() {
    input = "8";
    int programSize = 11;
    int program[] = {3,9,8,9,10,9,4,9,99,-1,8};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT(1, output);
}

//3,9,8,9,10,9,4,9,99,-1,8 - Using position mode, consider whether the input is equal to 8; output 1 (if it is) or 0 (if it is not).
void testEqualTo8IndirectWithNotEqualInput() {
    input = "6";
    int programSize = 11;
    int program[] = {3,9,8,9,10,9,4,9,99,-1,8};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT(0, output);
}

//3,9,7,9,10,9,4,9,99,-1,8 - Using position mode, consider whether the input is less than 8; output 1 (if it is) or 0 (if it is not).
void testLessThan8IndirectWithLessThanInput() {
    input = "6";
    int programSize = 11;
    int program[] = {3,9,7,9,10,9,4,9,99,-1,8};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT(1, output);
}

//3,9,7,9,10,9,4,9,99,-1,8 - Using position mode, consider whether the input is less than 8; output 1 (if it is) or 0 (if it is not).
void testLessThan8IndirectWithGreaterThanInput() {
    input = "8";
    int programSize = 11;
    int program[] = {3,9,7,9,10,9,4,9,99,-1,8};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT(0, output);
}

//3,3,1108,-1,8,3,4,3,99 - Using immediate mode, consider whether the input is equal to 8; output 1 (if it is) or 0 (if it is not).
void testEqualTo8DirectWithEqualInput() {
    input = "8";
    int programSize = 11;
    int program[] = {3,3,1108,-1,8,3,4,3,99};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT(1, output);
}

//3,3,1108,-1,8,3,4,3,99 - Using immediate mode, consider whether the input is equal to 8; output 1 (if it is) or 0 (if it is not).
void testEqualTo8DirectWithNotEqualInput() {
    input = "9";
    int programSize = 11;
    int program[] = {3,3,1108,-1,8,3,4,3,99};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT(0, output);
}

//3,3,1107,-1,8,3,4,3,99 - Using immediate mode, consider whether the input is less than 8; output 1 (if it is) or 0 (if it is not).
void testLessThan8DirectWithLessThanInput() {
    input = "7";
    int programSize = 9;
    int program[] = {3,3,1107,-1,8,3,4,3,99};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT(1, output);
}

//3,3,1107,-1,8,3,4,3,99 - Using immediate mode, consider whether the input is less than 8; output 1 (if it is) or 0 (if it is not).
void testLessThan8DirectWithGreaterThanInput() {
    input = "8";
    int programSize = 9;
    int program[] = {3,3,1107,-1,8,3,4,3,99};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT(0, output);
}

//3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9 (using position mode)
void testJumpsShouldOutput0With0InputIndirect() {
    input = "0";
    int programSize = 16;
    int program[] = {3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT(0, output);
}

//3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9 (using position mode)
void testJumpsShouldOutput1WithNonZeroInputIndirect() {
    input = "1";
    int programSize = 16;
    int program[] = {3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT(1, output);
}

//3,3,1105,-1,9,1101,0,0,12,4,12,99,1 (using immediate mode)
void testJumpsShouldOutput0With0InputDirect() {
    input = "0";
    int programSize = 13;
    int program[] = {3,3,1105,-1,9,1101,0,0,12,4,12,99,1};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT(0, output);
}

//3,3,1105,-1,9,1101,0,0,12,4,12,99,1 (using immediate mode)
void testJumpsShouldOutput1WithNonZeroInputDirect() {
    input = "1";
    int programSize = 13;
    int program[] = {3,3,1105,-1,9,1101,0,0,12,4,12,99,1};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT(1, output);
}

/**
 * 3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,
    1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,
    999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99
    The above example program uses an input instruction to ask for 
    a single number. The program will then output 999 if the input 
    value is below 8, output 1000 if the input value is equal to 8, 
    or output 1001 if the input value is greater than 8.
 */
void testProgramOutputs999WithInputLessThan8() {
    input = "1";
    int programSize = 47;
    int program[] = {3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,
    1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,
    999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT(999, output);
}

/**
 * 3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,
    1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,
    999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99
    The above example program uses an input instruction to ask for 
    a single number. The program will then output 999 if the input 
    value is below 8, output 1000 if the input value is equal to 8, 
    or output 1001 if the input value is greater than 8.
 */ 
void testProgramOutputs1000IfInputEqualTo8() {
    input = "8";
    int programSize = 47;
    int program[] = {3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,
    1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,
    999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT(1000, output);
}

/**
 * 3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,
    1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,
    999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99
    The above example program uses an input instruction to ask for 
    a single number. The program will then output 999 if the input 
    value is below 8, output 1000 if the input value is equal to 8, 
    or output 1001 if the input value is greater than 8.
 */ 
void testProgramOutputs1001IfInputGreaterThan8() {
    input = "9";
    int programSize = 47;
    int program[] = {3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,
    1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,
    999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT(1001, output);
}

/**
 * Here are some example programs:

    Max thruster signal 43210 (from phase setting sequence 4,3,2,1,0):

    3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0

    Max thruster signal 54321 (from phase setting sequence 0,1,2,3,4):

    3,23,3,24,1002,24,10,24,1002,23,-1,23,
    101,5,23,23,1,24,23,23,4,23,99,0,0

    Max thruster signal 65210 (from phase setting sequence 1,0,4,3,2):

    3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,
    1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0
 */ 

//run 

void testSequence43210ShouldReturn43210() {
    int programSize = 17;
    int program[] = {3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0};
    const char* inputs[] = {"4", "3", "2", "1", "0"};
    chainProgram(5, false, inputs, 0, program, programSize, standardReader, standardWriter);
    TEST_ASSERT_EQUAL_INT(43210, output);
}

void testSequence01234ShouldReturn54321() {
    int programSize = 25;
    int program[] = {3,23,3,24,1002,24,10,24,1002,23,-1,23,
    101,5,23,23,1,24,23,23,4,23,99,0,0};
    const char* inputs[] = {"0", "1", "2", "3", "4"};
    chainProgram(5, false, inputs, 0, program, programSize, standardReader, standardWriter);
    TEST_ASSERT_EQUAL_INT(54321, output);
}

void testSequence10432ShouldReturn65210() {
    int programSize = 34;
    int program[] = {3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,
    1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0};
    const char* inputs[] = {"1", "0", "4", "3", "2"};
    chainProgram(5, false, inputs, 0, program, programSize, standardReader, standardWriter);
    TEST_ASSERT_EQUAL_INT(65210, output);
}

void testWeActuallyGet65210AsMaxOutput() {
    int programSize = 34;
    int program[] = {3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,
    1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0};
    const char* inputs[] = {"1", "0", "4", "3", "2"};
    int result = decodeAmplifiers(5, false, 0, program, programSize, standardReader, standardWriter);
    TEST_ASSERT_EQUAL_INT(65210, result);
}

/**
 * 
 * Here are some example programs:

    Max thruster signal 139629729 (from phase setting sequence 9,8,7,6,5):

    3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,
    27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5

    Max thruster signal 18216 (from phase setting sequence 9,7,8,5,6):

    3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,
    -5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,
    53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10


 * 
 **/ 

/**
 * Max thruster signal 139629729 (from phase setting sequence 9,8,7,6,5):

    3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,
    27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5
 **/ 
void testWeActuallyGet139629729AsMaxOutputParallel() {
    int programSize = 29;
    int program[] = {3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,
    27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5};
    const char* inputs[] = {"4", "3", "2", "1", "0"};
    //5 amps, parallel mode, offset 5
    int result = decodeAmplifiers(5, 1, 5, program, programSize, standardReader, standardWriter);
    TEST_ASSERT_EQUAL_INT(139629729, result);
}

/**
 * Max thruster signal 18216 (from phase setting sequence 9,7,8,5,6):

    3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,
    -5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,
    53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10
 **/ 
void testWeActuallyGet18216AsMaxOutputParallel() {
    int programSize = 57;
    int program[] = {3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,
    -5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,
    53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10};
    const char* inputs[] = {"4", "2", "3", "0", "1"};
    //5 amps, parallel mode, offset 5
    int result = decodeAmplifiers(5, 1, 5, program, programSize, standardReader, standardWriter);
    TEST_ASSERT_EQUAL_INT(18216, result);
}

/**
 * Test cases for day 9 part 1
 * Here are some example programs that use these features:

    109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99 takes no input and produces a copy of itself as output.
    1102,34915192,34915192,7,4,7,99,0 should output a 16-digit number.
    104,1125899906842624,99 should output the large number in the middle.

 **/

/**
 * Opcode 9 adjusts the relative base by the value of its only parameter. 
 * The relative base increases (or decreases, if the value is negative) by the value of the parameter.
 **/ 
void opcode9ShouldAdjustRelativeBase() {
    int programSize = 3;
    int program[] = {109,20,99};
    executeProgram(initializeContext(program, programSize, queueReader, queueWriter));
    TEST_ASSERT_EQUAL_INT(20, 20);
}

//109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99 takes no input and produces a copy of itself as output.
void day9ProgramShouldProduceCopyOfItselfAsOutput() {
    int programSize = 16;
    int program[] = {109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99};
    executeProgram(initializeContext(program, programSize, queueReader, queueWriter));
    //check size of output collection
    TEST_ASSERT_EQUAL_INT(programSize, sizeQueue(outputQueue));
    //make sure every int in the collection is the same
    for (int i=0; i<programSize; i++) {
        TEST_ASSERT_EQUAL_INT(program[i], popQueue(outputQueue));
    }    
}

//1102,34915192,34915192,7,4,7,99,0 should output a 16-digit number.
void day9ProgramShouldOutput16DigitNumber() {
    int programSize = 8;
    int program[] = {1102,34915192,34915192,7,4,7,99,0};
    TEST_ASSERT_EQUAL_INT(0, 1);
}

//104,1125899906842624,99 should output the large number in the middle.
void day9ProgramShouldOutputNumberInMiddleOfProgram() {
    int programSize = 3;
    int program[] = {104,1125899906,99};
    TEST_ASSERT_EQUAL_INT(0, 1);
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(testParsingMixedModeInstruction);
    RUN_TEST(testParsingAddMixedModeInstruction);
    RUN_TEST(testParsingInputInstruction);
    RUN_TEST(testParsingOutputInstruction);
    RUN_TEST(testParsingHaltInstruction);
    RUN_TEST(testExecuteMixedModeInstruction);
    RUN_TEST(testInput);
    RUN_TEST(testOutput);
    RUN_TEST(testSimpleIOProgram);

    RUN_TEST(testEqualTo8IndirectWithEqualInput);
    RUN_TEST(testEqualTo8IndirectWithNotEqualInput);
    RUN_TEST(testLessThan8IndirectWithLessThanInput);
    RUN_TEST(testLessThan8IndirectWithGreaterThanInput);
    RUN_TEST(testEqualTo8DirectWithEqualInput);
    RUN_TEST(testEqualTo8DirectWithNotEqualInput);
    RUN_TEST(testLessThan8DirectWithLessThanInput);
    RUN_TEST(testLessThan8DirectWithGreaterThanInput);
    RUN_TEST(testJumpsShouldOutput0With0InputIndirect);
    RUN_TEST(testJumpsShouldOutput1WithNonZeroInputIndirect);
    RUN_TEST(testJumpsShouldOutput0With0InputDirect);
    RUN_TEST(testJumpsShouldOutput1WithNonZeroInputDirect);
    RUN_TEST(testProgramOutputs999WithInputLessThan8);
    RUN_TEST(testProgramOutputs1000IfInputEqualTo8);
    RUN_TEST(testProgramOutputs1001IfInputGreaterThan8);

    RUN_TEST(testSequence43210ShouldReturn43210);
    RUN_TEST(testSequence01234ShouldReturn54321);
    RUN_TEST(testSequence10432ShouldReturn65210);
    RUN_TEST(testWeActuallyGet65210AsMaxOutput);

    RUN_TEST(testWeActuallyGet139629729AsMaxOutputParallel);
    RUN_TEST(testWeActuallyGet18216AsMaxOutputParallel);

    RUN_TEST(day9ProgramShouldProduceCopyOfItselfAsOutput);
    RUN_TEST(day9ProgramShouldOutput16DigitNumber);
    RUN_TEST(day9ProgramShouldOutputNumberInMiddleOfProgram);
    return UNITY_END();
}
