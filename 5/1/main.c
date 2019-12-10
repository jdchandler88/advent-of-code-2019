#include "sol.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

static void programStringCallback(const char* programString) {
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

int main(int argc, char** argv) {
    if (argc <= 1) {
        exit(0);
    }
    FILE* file = fopen(argv[1], "r");
    readLines(file, programStringCallback);
    fclose(file);
}