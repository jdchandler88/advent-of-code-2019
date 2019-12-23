#include <unity.h>
#include <utils.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "sol.h"

int programCounter = 0;

const char* input;

const char** scrollingInput;

int currScrollingInput = 0;

program_t output;

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

void writeOutput(program_t programOutput, void* arg) {
    output = programOutput;
}

void writeScrollingOutput(program_t programOutput, void* arg) {
}

struct Queue* inputQueue;

struct Queue* outputQueue;

char outputBuffer[100];

const char* readQueue_test(void* arg) {
    struct Queue* queue = (struct Queue*)arg;
    program_t value = *(program_t*)queue_dequeue(queue).element.anyPointer;
    sprintf(outputBuffer, "%ld", value);
    return outputBuffer;
}

void writeQueue_test(program_t output, void* arg) {
    struct Queue* queue = (struct Queue*)arg;
    void* outputStorage = malloc(sizeof(program_t));
    memcpy(outputStorage, &output, sizeof(program_t));
    struct QueueElement outputElement = {VOIDP, {.anyPointer=outputStorage}};
    queue_enqueue(queue, outputElement);
}

void setUp() {
    programCounter = 0;
    input = NULL;
    scrollingInput = NULL;
    currScrollingInput = 0;

    inputQueue = queue_create();
    outputQueue = queue_create();

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

    
}

void tearDown() {
    free(context->program);
    free(context);
    free(standardReader);
    free(chainReader);
    free(queueReader);
    free(standardWriter);
    free(chainWriter);
    free(queueWriter);

    queue_destroy(inputQueue);
    queue_destroy(outputQueue);
}

struct ProgramContext* initializeContext(program_t* program, int programSize, InputReader* standardReader, OutputWriter* standardWriter) {
    context->programCounter = 0;
    context->relativeBase = 0;
    program_t* programCopy = calloc(1000+programSize, sizeof(program_t));
    memcpy(programCopy, program, programSize*sizeof(program_t));
    context->program = programCopy;
    context->programLength = programSize;
    context->reader = standardReader;
    context->writer = standardWriter;
}

void testParsingMixedModeInstruction() {
    int instructionInt = 1002;
    //parse the opcode into a struct
    struct Instruction* instruction = parseInstruction(instructionInt);
    //make the assertions
    TEST_ASSERT_EQUAL_INT64(2, instruction->opcode);
    TEST_ASSERT_EQUAL_INT64(0, instruction->parameter->mode);
    TEST_ASSERT_EQUAL_INT64(1, instruction->parameter->next->mode);
    TEST_ASSERT_EQUAL_INT64(0, instruction->parameter->next->next->mode);
}

void testParsingAddMixedModeInstruction() {
    int instructionInt = 10101;
    //parse the opcode into a struct
    struct Instruction* instruction = parseInstruction(instructionInt);
    //make the assertions
    TEST_ASSERT_EQUAL_INT64(1, instruction->opcode);
    TEST_ASSERT_EQUAL_INT64(1, instruction->parameter->mode);
    TEST_ASSERT_EQUAL_INT64(0, instruction->parameter->next->mode);
    TEST_ASSERT_EQUAL_INT64(1, instruction->parameter->next->next->mode);
}

void testParsingInputInstruction() {
    int instructionInt = 03;
    //parse the opcode into a struct
    struct Instruction* instruction = parseInstruction(instructionInt);
    //make the assertions
    TEST_ASSERT_EQUAL_INT64(3, instruction->opcode);
    TEST_ASSERT_EQUAL_INT64(0, instruction->parameter->mode);
}

void testParsingOutputInstruction() {
    int instructionInt = 04;
    //parse the opcode into a struct
    struct Instruction* instruction = parseInstruction(instructionInt);
    //make the assertions
    TEST_ASSERT_EQUAL_INT64(4, instruction->opcode);
    TEST_ASSERT_EQUAL_INT64(0, instruction->parameter->mode);
}

void testParsingHaltInstruction() {
    int instructionInt = 99;
    //parse the opcode into a struct
    struct Instruction* instruction = parseInstruction(instructionInt);
    //make the assertions
    TEST_ASSERT_EQUAL_INT64(99, instruction->opcode);
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
    program_t program[] = {1002,4,3,4,33};
    program_t expected[] = {1002,4,3,4,99};
    //execute program
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64_ARRAY(expected, context->program, programSize);
}

/**
 * Opcode 3 takes a single integer as input and saves it to the position 
    given by its only parameter. For example, the instruction 3,50 would 
    take an input value and store it at address 50.
**/ 
void testInput() {
    int programSize = 3;
    program_t program[] = {3,1,99};
    program_t expected[] = {3,1234,99};
    //test input
    input = "1234";
    //execute program
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64_ARRAY(expected, context->program, programSize);
}

/**
 * Opcode 4 outputs the value of its only parameter. For example, the 
    instruction 4,50 would output the value at address 50.
**/
void testOutput() {
    int programSize = 3;
    program_t program[] = {4,0,99};
    //execute program
    //need to add 'ouptput writer' that will capture output to wherever we specify
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64(4, output);
}

/**
 * Opcode 3 takes a single integer as input and saves it to the position 
    given by its only parameter. For example, the instruction 3,50 would 
    take an input value and store it at address 50.
**/
void testSimpleIOProgram() {
    int programSize = 5;
    program_t program[] = {3,0,4,0,99};
    program_t expected[] = {4321,0,4,0,99};
    input = "4321";
    //execute program
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64(4321, output);
    TEST_ASSERT_EQUAL_INT64_ARRAY(expected, context->program, programSize);
}

//3,9,8,9,10,9,4,9,99,-1,8 - Using position mode, consider whether the input is equal to 8; output 1 (if it is) or 0 (if it is not).
void testEqualTo8IndirectWithEqualInput() {
    input = "8";
    int programSize = 11;
    program_t program[] = {3,9,8,9,10,9,4,9,99,-1,8};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64(1, output);
}

//3,9,8,9,10,9,4,9,99,-1,8 - Using position mode, consider whether the input is equal to 8; output 1 (if it is) or 0 (if it is not).
void testEqualTo8IndirectWithNotEqualInput() {
    input = "6";
    int programSize = 11;
    program_t program[] = {3,9,8,9,10,9,4,9,99,-1,8};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64(0, output);
}

//3,9,7,9,10,9,4,9,99,-1,8 - Using position mode, consider whether the input is less than 8; output 1 (if it is) or 0 (if it is not).
void testLessThan8IndirectWithLessThanInput() {
    input = "6";
    int programSize = 11;
    program_t program[] = {3,9,7,9,10,9,4,9,99,-1,8};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64(1, output);
}

//3,9,7,9,10,9,4,9,99,-1,8 - Using position mode, consider whether the input is less than 8; output 1 (if it is) or 0 (if it is not).
void testLessThan8IndirectWithGreaterThanInput() {
    input = "8";
    int programSize = 11;
    program_t program[] = {3,9,7,9,10,9,4,9,99,-1,8};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64(0, output);
}

//3,3,1108,-1,8,3,4,3,99 - Using immediate mode, consider whether the input is equal to 8; output 1 (if it is) or 0 (if it is not).
void testEqualTo8DirectWithEqualInput() {
    input = "8";
    int programSize = 11;
    program_t program[] = {3,3,1108,-1,8,3,4,3,99};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64(1, output);
}

//3,3,1108,-1,8,3,4,3,99 - Using immediate mode, consider whether the input is equal to 8; output 1 (if it is) or 0 (if it is not).
void testEqualTo8DirectWithNotEqualInput() {
    input = "9";
    int programSize = 11;
    program_t program[] = {3,3,1108,-1,8,3,4,3,99};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64(0, output);
}

//3,3,1107,-1,8,3,4,3,99 - Using immediate mode, consider whether the input is less than 8; output 1 (if it is) or 0 (if it is not).
void testLessThan8DirectWithLessThanInput() {
    input = "7";
    int programSize = 9;
    program_t program[] = {3,3,1107,-1,8,3,4,3,99};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64(1, output);
}

//3,3,1107,-1,8,3,4,3,99 - Using immediate mode, consider whether the input is less than 8; output 1 (if it is) or 0 (if it is not).
void testLessThan8DirectWithGreaterThanInput() {
    input = "8";
    int programSize = 9;
    program_t program[] = {3,3,1107,-1,8,3,4,3,99};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64(0, output);
}

//3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9 (using position mode)
void testJumpsShouldOutput0With0InputIndirect() {
    input = "0";
    int programSize = 16;
    program_t program[] = {3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64(0, output);
}

//3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9 (using position mode)
void testJumpsShouldOutput1WithNonZeroInputIndirect() {
    input = "1";
    int programSize = 16;
    program_t program[] = {3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64(1, output);
}

//3,3,1105,-1,9,1101,0,0,12,4,12,99,1 (using immediate mode)
void testJumpsShouldOutput0With0InputDirect() {
    input = "0";
    int programSize = 13;
    program_t program[] = {3,3,1105,-1,9,1101,0,0,12,4,12,99,1};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64(0, output);
}

//3,3,1105,-1,9,1101,0,0,12,4,12,99,1 (using immediate mode)
void testJumpsShouldOutput1WithNonZeroInputDirect() {
    input = "1";
    int programSize = 13;
    program_t program[] = {3,3,1105,-1,9,1101,0,0,12,4,12,99,1};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64(1, output);
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
    program_t program[] = {3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,
    1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,
    999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64(999, output);
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
    program_t program[] = {3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,
    1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,
    999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64(1000, output);
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
    program_t program[] = {3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,
    1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,
    999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99};
    executeProgram(initializeContext(program, programSize, standardReader, standardWriter));
    TEST_ASSERT_EQUAL_INT64(1001, output);
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
    program_t program[] = {3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0};
    const char* inputs[] = {"4", "3", "2", "1", "0"};
    chainProgram(5, false, inputs, 0, program, programSize, standardReader, standardWriter);
    TEST_ASSERT_EQUAL_INT64(43210, output);
}

void testSequence01234ShouldReturn54321() {
    int programSize = 25;
    program_t program[] = {3,23,3,24,1002,24,10,24,1002,23,-1,23,
    101,5,23,23,1,24,23,23,4,23,99,0,0};
    const char* inputs[] = {"0", "1", "2", "3", "4"};
    chainProgram(5, false, inputs, 0, program, programSize, standardReader, standardWriter);
    TEST_ASSERT_EQUAL_INT64(54321, output);
}

void testSequence10432ShouldReturn65210() {
    int programSize = 34;
    program_t program[] = {3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,
    1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0};
    const char* inputs[] = {"1", "0", "4", "3", "2"};
    chainProgram(5, false, inputs, 0, program, programSize, standardReader, standardWriter);
    TEST_ASSERT_EQUAL_INT64(65210, output);
}

void testWeActuallyGet65210AsMaxOutput() {
    int programSize = 34;
    program_t program[] = {3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,
    1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0};
    const char* inputs[] = {"1", "0", "4", "3", "2"};
    int result = decodeAmplifiers(5, false, 0, program, programSize, standardReader, standardWriter);
    TEST_ASSERT_EQUAL_INT64(65210, result);
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
    program_t program[] = {3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,
    27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5};
    const char* inputs[] = {"4", "3", "2", "1", "0"};
    //5 amps, parallel mode, offset 5
    int result = decodeAmplifiers(5, 1, 5, program, programSize, standardReader, standardWriter);
    TEST_ASSERT_EQUAL_INT64(139629729, result);
}

/**
 * Max thruster signal 18216 (from phase setting sequence 9,7,8,5,6):

    3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,
    -5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,
    53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10
 **/ 
void testWeActuallyGet18216AsMaxOutputParallel() {
    int programSize = 57;
    program_t program[] = {3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,
    -5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,
    53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10};
    const char* inputs[] = {"4", "2", "3", "0", "1"};
    //5 amps, parallel mode, offset 5
    int result = decodeAmplifiers(5, 1, 5, program, programSize, standardReader, standardWriter);
    TEST_ASSERT_EQUAL_INT64(18216, result);
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
    int programSize = 5;
    program_t program[] = {109,20,9,0,99};
    executeProgram(initializeContext(program, programSize, queueReader, queueWriter));
    //first adds 20 (direct), then adds 109 (indirect). 129 final
    TEST_ASSERT_EQUAL_INT64(129, context->relativeBase);
}

//there is a bug with parsing instructions in relative mode. value of 2 for mode is not currently returned. let's get a test in there to focus on it
void testParsingRelativeInstructionReturnsAppropriateValue() {
    int instructionInt = 204;
    //parse the opcode into a struct
    struct Instruction* instruction = parseInstruction(instructionInt);
    //make the assertions
    TEST_ASSERT_EQUAL_INT64(4, instruction->opcode);
    TEST_ASSERT_EQUAL_INT64(2, instruction->parameter->mode);
}

/**
 * reddit test casees to help debug my code
 * [109, -1, 4, 1, 99] outputs -1

[109, -1, 104, 1, 99] outputs 1

[109, -1, 204, 1, 99] outputs 109

[109, 1, 9, 2, 204, -6, 99] outputs 204

[109, 1, 109, 9, 204, -6, 99] outputs 204

[109, 1, 209, -1, 204, -106, 99] outputs 204

[109, 1, 3, 3, 204, 2, 99] outputs the input

[109, 1, 203, 2, 204, 2, 99] outputs the input
 **/ 

void outputShouldBeNegative1() {
    int programSize = 5;
    program_t program[] = {109, -1, 4, 1, 99};
    executeProgram(initializeContext(program, programSize, queueReader, queueWriter));
    TEST_ASSERT_EQUAL_INT64(-1, *(program_t*)queue_dequeue(outputQueue).element.anyPointer);
}

void outputShouldBe1() {
    int programSize = 5;
    program_t program[] = {109, -1, 104, 1, 99};
    executeProgram(initializeContext(program, programSize, queueReader, queueWriter));
    TEST_ASSERT_EQUAL_INT64(1, *(program_t*)queue_dequeue(outputQueue).element.anyPointer);
}

void outputShouldBe109() {
    int programSize = 5;
    program_t program[] = {109, -1, 204, 1, 99};
    executeProgram(initializeContext(program, programSize, queueReader, queueWriter));
    TEST_ASSERT_EQUAL_INT64(109, *(program_t*)queue_dequeue(outputQueue).element.anyPointer);
}

void outputShouldBe204_1() {
    int programSize = 7;
    program_t program[] = {109, 1, 9, 2, 204, -6, 99};
    executeProgram(initializeContext(program, programSize, queueReader, queueWriter));
    TEST_ASSERT_EQUAL_INT64(204, *(program_t*)queue_dequeue(outputQueue).element.anyPointer);
}

void outputShouldBe204_2() {
    int programSize = 7;
    program_t program[] = {109, 1, 109, 9, 204, -6, 99};
    executeProgram(initializeContext(program, programSize, queueReader, queueWriter));
    TEST_ASSERT_EQUAL_INT64(204, *(program_t*)queue_dequeue(outputQueue).element.anyPointer);
}

void outputShouldBe204_3() {
    int programSize = 7;
    program_t program[] = {109, 1, 209, -1, 204, -106, 99};
    executeProgram(initializeContext(program, programSize, queueReader, queueWriter));
    TEST_ASSERT_EQUAL_INT64(204, *(program_t*)queue_dequeue(outputQueue).element.anyPointer);
}

void outputShouldBeInput_1() {
    int programSize = 7;
    program_t program[] = {109, 1, 3, 3, 204, 2, 99};
    int input = 25;
    writeQueue_test(25, inputQueue);
    executeProgram(initializeContext(program, programSize, queueReader, queueWriter));
    TEST_ASSERT_EQUAL_INT64(input, *(program_t*)queue_dequeue(outputQueue).element.anyPointer);
}

void outputShouldBeInput_2() {
    int programSize = 7;
    program_t program[] = {109, 1, 203, 2, 204, 2, 99};
    int input = 25;
    writeQueue_test(25, inputQueue);
    executeProgram(initializeContext(program, programSize, queueReader, queueWriter));
    TEST_ASSERT_EQUAL_INT64(input, *(program_t*)queue_dequeue(outputQueue).element.anyPointer);
}

//109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99 takes no input and produces a copy of itself as output.
void day9ProgramShouldProduceCopyOfItselfAsOutput() {
    int programSize = 16;
    program_t program[] = {109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99};
    executeProgram(initializeContext(program, programSize, queueReader, queueWriter));
    //check size of output collection
    TEST_ASSERT_EQUAL_INT64(programSize, queue_size(outputQueue));
    //make sure every int in the collection is the same
    for (int i=0; i<programSize; i++) {
        TEST_ASSERT_EQUAL_INT64(program[i], *(program_t*)queue_dequeue(outputQueue).element.anyPointer);
    }    
}

//1102,34915192,34915192,7,4,7,99,0 should output a 16-digit number.
void day9ProgramShouldOutput16DigitNumber() {
    int programSize = 8;
    program_t program[] = {1102,34915192,34915192,7,4,7,99,0};
    executeProgram(initializeContext(program, programSize, queueReader, queueWriter));
    char buf[100];
    sprintf(buf, "%ld", *(program_t*)queue_dequeue(outputQueue).element.anyPointer);
    TEST_ASSERT_EQUAL_INT64(16, strlen(buf));
}

//104,1125899906842624,99 should output the large number in the middle.
void day9ProgramShouldOutputNumberInMiddleOfProgram() {
    int programSize = 3;
    program_t program[] = {104,1125899906842624,99};
    executeProgram(initializeContext(program, programSize, queueReader, queueWriter));
    TEST_ASSERT_EQUAL_INT64(1125899906842624, *(program_t*)queue_dequeue(outputQueue).element.anyPointer);
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

    RUN_TEST(opcode9ShouldAdjustRelativeBase);
    RUN_TEST(testParsingRelativeInstructionReturnsAppropriateValue);
    RUN_TEST(outputShouldBeNegative1);
    RUN_TEST(outputShouldBe1);
    RUN_TEST(outputShouldBe109);
    RUN_TEST(outputShouldBe204_1);
    RUN_TEST(outputShouldBe204_2);
    RUN_TEST(outputShouldBe204_3);
    RUN_TEST(outputShouldBeInput_1);
    RUN_TEST(outputShouldBeInput_2);
    RUN_TEST(day9ProgramShouldProduceCopyOfItselfAsOutput);
    RUN_TEST(day9ProgramShouldOutput16DigitNumber);
    RUN_TEST(day9ProgramShouldOutputNumberInMiddleOfProgram);
    return UNITY_END();
}
