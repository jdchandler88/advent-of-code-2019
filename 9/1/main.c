#include "sol.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int input;

char inputBuffer[100];

struct Queue* inputQueue;

struct Queue* outputQueue;

const char* readInput(void* arg) {
    struct Queue* queue = (Queue*)arg;
    program_t value = *((program_t*)queue_dequeue(queue).element.anyPointer);
    sprintf(inputBuffer, "%ld", value);
    return inputBuffer;
}

void writeOutput(program_t output, void* arg) {
    struct Queue* queue = (Queue*)arg;
    //create storage and copy value into it
    void* outputPointer = malloc(sizeof(program_t));
    memcpy(outputPointer, &output, sizeof(program_t));
    //now that value is copied, push it into the queue
    struct QueueElement outputElement = {VOIDP, {.anyPointer=outputPointer}};
    queue_enqueue(queue, outputElement);
    printf("output: %ld\n", output);
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
    reader.readerContext = inputQueue;
    OutputWriter writer;
    writer.writer = writeOutput;
    writer.writerContext = outputQueue;


    struct ProgramContext* ctx = createContext(0, program, programSize, &reader, &writer);
    executeProgram(ctx);
    //free the program storage
    free((void*)program);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("gimme an input file, number of amplifiers we're decoding, feedbackMode, inputOffset. For example './exe input.txt 5 1 0'\n");
        printf("\tinputFile should be a string pointing to a file\n");
        printf("\tinput. this is used to pass to the BOOST program\n");
        exit(0);
    }
    FILE* file = fopen(argv[1], "r");
    input = atoi(argv[2]);

    //set up io
    inputQueue = queue_create();
    outputQueue = queue_create();

    //push input into input quuee
    void* outputPointer = malloc(sizeof(program_t));
    memcpy(outputPointer, &input, sizeof(program_t));
    //now that value is copied, push it into the queue
    struct QueueElement outputElement = {VOIDP, {.anyPointer=outputPointer}};
    queue_enqueue(inputQueue, outputElement);

    readLines(file, programStringCallback);
    fclose(file);
}