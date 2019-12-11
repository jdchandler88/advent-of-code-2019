#include "sol.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

InputReader programReader;
OutputWriter programWriter;

static void handleAdd(struct Instruction* instruction, int* program, int* programCounter) {
    (*programCounter)++;
    int memParam1 = program[(*programCounter)++];
    int addend1 = instruction->parameter->mode == DIRECT ? memParam1 : program[memParam1];

    int memParam2 = program[(*programCounter)++];
    int addend2 = instruction->parameter->next->mode == DIRECT ? memParam2 : program[memParam2];

    int storeLocation = program[(*programCounter)++];

    int result = addend1 + addend2;
    program[storeLocation] = result;
};

static void handleMultiply(struct Instruction* instruction, int* program, int* programCounter) {
    (*programCounter)++;
    int memParam1 = program[(*programCounter)++];
    int addend1 = instruction->parameter->mode == DIRECT ? memParam1 : program[memParam1];

    int memParam2 = program[(*programCounter)++];
    int addend2 = instruction->parameter->next->mode == DIRECT ? memParam2 : program[memParam2];

    int storeLocation = program[(*programCounter)++];

    int result = addend1 * addend2;
    program[storeLocation] = result;
}

static void handleInput(struct Instruction* instruction, int* program, int* programCounter) {
    (*programCounter)++;
    //read input from somewhere
    int storeLocation = program[(*programCounter)++];
    const char* inputString = programReader();
    int input = atoi(inputString);
    //store input at location specified by program
    program[storeLocation] = input;
}

static void handleOutput(struct Instruction* instruction, int* program, int* programCounter) {
    (*programCounter)++;
    //get value to output
    int outputLocation = program[(*programCounter)++];
    int output = program[outputLocation];
    //write it to somewhere
    programWriter(output);
}

static int getNumberOfParams(enum OpCode opcode) {
    switch (opcode) {
        case ADD:
            return ADD_PARAMS; 
        case MUL:
            return MUL_PARAMS;
        case IN:
            return IN_PARAMS;
        case OUT:
            return OUT_PARAMS;
        case HALT:
            return HALT_PARAMS;
    }
}

/**
 *  * ABCDE
    1002

DE - two-digit opcode,      02 == opcode 2
 C - mode of 1st parameter,  0 == position mode
 B - mode of 2nd parameter,  1 == immediate mode
 A - mode of 3rd parameter,  0 == position mode,
                                  omitted due to being a leading zero

...I deifnitely got lazy. Tired of string parsing in C. No me gusta.
The algorithm here is: mod by an order of magnitude higher than the information you wnat.
For instance: i want the 10s/1s place. Mod the int by 100 and that gives me the 10s/1s place.
For the next slot (100s), mod by the next order of magnitude, 1000. And so it goes...
*/                                
struct Instruction* parseInstruction(int instructionInt) {
    int intMask = 100;
    int tmp = instructionInt%intMask;
    int opcode = tmp;
    instructionInt-=opcode;
    intMask*=10;    //incrase mask for next base10 place0

    struct Instruction* instruction = malloc(sizeof(struct Instruction));
    instruction->opcode = opcode;
    instruction->parameter = NULL;

    //storage for parameter list
    struct Parameter* params = malloc(sizeof(struct Parameter));
    int numParams = getNumberOfParams(opcode);
    struct Parameter* currentParameter = NULL;
    for (int i=0; i<numParams; i++) {
        tmp = instructionInt%intMask;
        int mode = tmp > 0;
        instructionInt-=tmp;
        intMask*=10;
        //create storage for mode indicator
        struct Parameter* newParameter = malloc(sizeof(struct Parameter));
        newParameter->mode = mode;
        //if it's new, initialize instruction with parameter. otherwise, update the linked list
        if (currentParameter == NULL) {
            instruction->parameter = newParameter;
            currentParameter = newParameter;
        } else {
            currentParameter->next = newParameter;
            currentParameter = newParameter;
        }
    }
    return instruction;
}

static void printIntArray(int* arr, int size) {
    for (int i=0; i<size; i++) {
        if (i==0) {
            printf("[");
        }
        printf("%i", arr[i]);
        if (i!=size-1) {
            printf(", ");
        } else {
            printf("]");
        }
    }
    printf("\n");
}

void executeProgram(int* program, int programLength, InputReader reader, OutputWriter writer) {
    //store reader and writer
    programReader = reader;
    programWriter = writer;
    int programCounter = 0;
    while (program[programCounter] != HALT) {
        executeInstruction(program, &programCounter);
    }
    printf("result = ");
    printIntArray(program, programLength);
}

struct Parameter* nextParameter(struct Parameter* currentParameter) {
    return currentParameter->next;
}

void executeInstruction(int* program, int* programCounter) {
    struct Instruction* instruction = parseInstruction(program[*programCounter]);
    switch (instruction->opcode) {
        case ADD:
            handleAdd(instruction, program, programCounter);
            break;
        case MUL:
            handleMultiply(instruction, program, programCounter);
            break;
        case  IN:
            handleInput(instruction, program, programCounter);
            break;
        case OUT:
            handleOutput(instruction, program, programCounter);
            break;
        default:
            printf("undefined OPCODE %i. Quitting...\n", instruction->opcode);
            exit(1);
            //what to do here? should probably stick a halt in the program causing a halt.
            //could also exit. 
    }
    freeLinkedList((void*)instruction->parameter, (void*(*)(void*))nextParameter);
    free(instruction);
}