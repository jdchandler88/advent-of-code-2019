#include "intcode.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int desiredOutput;

static void programStringCallback(const char* programString) {
    //parse the program
    int programSize;
    const char** parsedProgram = parseString(programString, ",", &programSize);
    //convert to integers and delete character storage
    int* program = malloc(programSize*sizeof(int));
    //keep copy of program so we can restore after invokations
    int* programCopy = malloc(programSize*sizeof(int));
    for (int i=0; i<programSize; i++) {
        int parsedInt = atoi(parsedProgram[i]);
        program[i] = parsedInt;
        programCopy[i] = parsedInt;
        free((void*)parsedProgram[i]);
    }
    free((void*)parsedProgram);

    /*
    * The inputs should still be provided to the program by replacing 
    * the values at addresses 1 and 2, just like before. In this program,
    * the value placed in address 1 is called the noun, and the value 
    * placed in address 2 is called the verb. Each of the two input values 
    * will be between 0 and 99, inclusive.
    */
    for (int noun = 0; noun <= 99; noun++) {
        for (int verb = 0; verb <= 99; verb++) {
            program[1] = noun;
            program[2] = verb;
            executeProgram(program, programSize);
            if (program[0] == desiredOutput) {
                printf("noun=%i, verb=%i, 100*noun+verb=%i\n", noun, verb, 100*noun+verb);
                break;
            }
            //reset program to original state
            for (int i=0; i<programSize; i++) {
                program[i] = programCopy[i];
            }
        }
    }
    //free the program storage
    free((void*)program);
    free((void*)programCopy);
}

int main(int argc, char** argv) {
    if (argc != 3) {    //need name of program (always there), file, and desiredOutput. IN THAT ORDER
        exit(0);
    }
    FILE* file = fopen(argv[1], "r");
    desiredOutput = atoi(argv[2]);
    readLines(file, programStringCallback);
    fclose(file);
}