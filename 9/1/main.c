#include "sol.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int numAmps;

int feedbackMode = false;

int inputOffset = 0;

const char* readInput(void* arg) {
    printf("Please enter your input...\n");
    char * line = NULL;
    size_t len = 0;
    size_t read;
    getline(&line, &len, stdin);
    return line;
}

void writeOutput(program_t output, void* arg) {
    printf("This is the program output: %ld\n", output);
}

static void programStringCallback(const char* programString) {
    //parse the program
    int programSize;
    const char** parsedProgram = parseString(programString, ",", &programSize);
    //convert to integers and delete character storage
    program_t* program = malloc(programSize*sizeof(program_t));
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
    if (argc !=5) {
        printf("gimme an input file, number of amplifiers we're decoding, feedbackMode, inputOffset. For example './exe input.txt 5 1 0'\n");
        printf("\tinputFile should be a string pointing to a file\n");
        printf("\tnumAmps should be an integer indicating the number of systems running the program\n");
        printf("\tfeedbackMode should be 0 or 1. If '0', then feedback mode is OFF and programs are chained sequentially. If '1' then feedback mode is on and programs are executed in parallel and connected in a feedback loop\n");
        printf("\tinputOffset should be an integer >= 0. This program generates permutations of inputs for the amps, 0 indexed. 'inputOffset' is added to the generated permutations'\n");
        exit(0);
    }
    FILE* file = fopen(argv[1], "r");
    numAmps = atoi(argv[2]);
    feedbackMode = atoi(argv[3]);
    if (!(feedbackMode == 0 || feedbackMode == 1)) {
        printf("feedbackMode should be '0' or '1'. We received %i\n", feedbackMode);
        exit(1);
    }
    inputOffset = atoi(argv[4]);
    if (inputOffset < 0) {
        printf("inputOffset should be integer >= 0. We received %i\n", inputOffset);
        exit(1);
    }
    readLines(file, programStringCallback);
    fclose(file);
}