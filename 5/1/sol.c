#include "sol.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

static void handleAdd(int* program, int* programCounter) {
    int addend1Location = program[++*programCounter];
    int addend2Location = program[++*programCounter];
    int storeLocation = program[++*programCounter];
    int result = program[addend1Location] + program[addend2Location];
    program[storeLocation] = result;
    ++*programCounter;  //advance to next opcode
};

static void handleMultiply(int* program, int* programCounter) {
    int multiplicand1Location = program[++*programCounter];
    int multiplicand2Location = program[++*programCounter];
    int storeLocation = program[++*programCounter];
    int result = program[multiplicand1Location] * program[multiplicand2Location];
    program[storeLocation] = result;
    ++*programCounter;  //advance to next opcode
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

void executeProgram(int* program, int programSize) {
    int programCounter = 0;
    while (program[programCounter] != HALT) {
        executeInstruction(program, &programCounter);
    }
    printf("result = ");
    printIntArray(program, programSize);
}

void executeInstruction(int* program, int* programCounter) {
    int opCode = program[*programCounter];
    printf("opCode = %i\n", opCode);
    switch (opCode) {
        case ADD:
            handleAdd(program, programCounter);
            break;
        case MUL:
            handleMultiply(program, programCounter);
            break;
        default:
            printf("undefined OPCODE %i. Quitting...\n", opCode);
            exit(1);
            //what to do here? should probably stick a halt in the program causing a halt.
            //could also exit. 
    }
}