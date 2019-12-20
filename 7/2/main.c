#include "sol.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int numAmps;

bool feedbackMode = false;

int inputOffset = 0;

const char* readInput(void* arg) {
    printf("Please enter your input...\n");
    char * line = NULL;
    size_t len = 0;
    size_t read;
    getline(&line, &len, stdin);
    return line;
}

void writeOutput(int output, void* arg) {
    printf("This is the program output: %i\n", output);
}

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
    InputReader reader;
    reader.reader = readInput;
    reader.readerContext = NULL;
    OutputWriter writer;
    writer.writer = writeOutput;
    writer.writerContext = NULL;
    decodeAmplifiers(numAmps, feedbackMode, inputOffset, program, programSize, &reader, &writer);
    //free the program storage
    free((void*)program);
}

int main(int argc, char** argv) {
    if (argc !=3) {
        printf("gimme an input file and the number of amplifiers we're decoding. For example './exe input.txt 5'\n");
        exit(0);
    }
    FILE* file = fopen(argv[1], "r");
    numAmps = atoi(argv[2]);
    readLines(file, programStringCallback);
    fclose(file);
}