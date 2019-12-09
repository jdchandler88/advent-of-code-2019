#include "intcode.h"
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
            exit(1);
            //what to do here? should probably stick a halt in the program causing a halt.
            //could also exit. 
    }
}

void programStringCallback(const char* programString) {
    //parse the program
    int programSize;
    const char** parsedProgram = parseString(programString, ",", &programSize);
    //convert to integers and delete character storage
    int* program = malloc(programSize*sizeof(int));
    for (int i=0; i<programSize; i++) {
        program[i] = atoi(parsedProgram[i]);
        free((void*)parsedProgram[i]);
    }
    free((void*)parsedProgram);
    //do the program
    executeProgram(program, programSize);
    //free the program storage
    free((void*)program);
}

#ifndef TEST
int main(int argc, char** argv) {
    if (argc <= 1) {
        exit(0);
    }
    FILE* file = fopen(argv[1], "r");
    readLines(file, programStringCallback);
    fclose(file);
}
#endif