#include "sol.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

InputReader programReader;
OutputWriter programWriter;

static void handleAdd(struct Instruction* instruction, int* program, int* programCounter) {
    (*programCounter)++;
    int memParam1 = program[(*programCounter)++];
    int addend1 = instruction->parameter->mode == DIRECT ? memParam1 : program[memParam1];

    int memParam2 = program[(*programCounter)++];
    int addend2 = instruction->parameter->next->mode == DIRECT ? memParam2 : program[memParam2];

    // int memParam3 = program[(*programCounter)++];
    // int storeLocation = instruction->parameter->next->next->mode == DIRECT ? memParam3 : program[memParam3];
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
    int memParam = program[(*programCounter)++];
    int output = instruction->parameter->mode == DIRECT ? memParam : program[memParam];
    //write it to somewhere
    programWriter(output);
}

static void handleJumpIfTrue(struct Instruction* instruction, int* program, int* programCounter) {
    (*programCounter)++;
    int memoryElement1 = program[(*programCounter)++];
    int value1 = instruction->parameter->mode == DIRECT ? memoryElement1 : program[memoryElement1];

    int memoryElement2 = program[(*programCounter)++];
    int jumpLocation = instruction->parameter->next->mode == DIRECT ? memoryElement2 : program[memoryElement2];

    if (value1!=0) {
        *programCounter = jumpLocation;
    }
}

static void handleJumpIfFalse(struct Instruction* instruction, int* program, int* programCounter) {
    (*programCounter)++;
    int memoryElement1 = program[(*programCounter)++];
    int value1 = instruction->parameter->mode == DIRECT ? memoryElement1 : program[memoryElement1];

    int memoryElement2 = program[(*programCounter)++];
    int jumpLocation = instruction->parameter->next->mode == DIRECT ? memoryElement2 : program[memoryElement2];

    if (value1==0) {
        *programCounter = jumpLocation;
    }
}

static void handleLessThan(struct Instruction* instruction, int* program, int* programCounter) {
    (*programCounter)++;
    int memoryElement1 = program[(*programCounter)++];
    int value1 = instruction->parameter->mode == DIRECT ? memoryElement1 : program[memoryElement1];
    
    int memoryElement2 = program[(*programCounter)++];
    int value2 = instruction->parameter->next->mode == DIRECT ? memoryElement2 : program[memoryElement2];

    int storeLocation = program[(*programCounter)++];

    program[storeLocation] = (value1<value2);
}

static void handleEquals(struct Instruction* instruction, int* program, int* programCounter) {
    (*programCounter)++;
    int memoryElement1 = program[(*programCounter)++];
    int value1 = instruction->parameter->mode == DIRECT ? memoryElement1 : program[memoryElement1];
    
    int memoryElement2 = program[(*programCounter)++];
    int value2 = instruction->parameter->next->mode == DIRECT ? memoryElement2 : program[memoryElement2];
    
    int storeLocation = program[(*programCounter)++];
    
    program[storeLocation] = (value1==value2);
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
        case JUMP_IF_TRUE:
            return JUMP_IF_TRUE_PARAMS;
        case JUMP_IF_FALSE:
            return JUMP_IF_FALSE_PARAMS;
        case LESS_THAN:
            return LESS_THAN_PARAMS;
        case EQUALS:
            return EQUAL_PARAMS;
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
        newParameter->next = NULL;
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
    int pc = *programCounter;
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
        case JUMP_IF_TRUE:
            handleJumpIfTrue(instruction, program, programCounter);
            break;
        case JUMP_IF_FALSE:
            handleJumpIfFalse(instruction, program, programCounter);
            break;
        case LESS_THAN:
            handleLessThan(instruction, program, programCounter);
            break;
        case EQUALS:
            handleEquals(instruction, program, programCounter);
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
    //freeLinkedList((void*)instruction->parameter, (void*(*)(void*))nextParameter);
    //free(instruction);
}


/**
 * BEGIN CODE FOR DAY 7 PART 1
 **/ 
 
static void copyProgram(int* storageLocation, int* program, int programLength) {
   memcpy(storageLocation, program, programLength * sizeof(int));
}

static int chainOutput;
/**
 * This output writer stores the output for intermediate programs. This output is then
 * used by the reader below. This is the mechanism for passing output between programs.
 */ 
static void writeOutputForChain(int output) {
    printf("outputting: %i\n", output);
    chainOutput = output;
}

static const char* inputForChain;
static char* signalString;
bool phaseNotSignal = true;
/**
 * this reader toggles between reading input from "user" and from "output". This makes this program automatable.
 * The first thing to read is the phase [0,4]. The second thing to read is amplifier signal. Then the next program 
 * needs to do the same thing. And so on. This is the mechanism for passing output between programs.
 */ 
static const char* readInputForChain() {
    if (phaseNotSignal) {
        printf("readingPhase: %s\n", inputForChain);
        phaseNotSignal = !phaseNotSignal;
        return inputForChain;
    } else {
        sprintf(signalString, "%i", chainOutput);
        printf("readingSignal: int=%i, str=%s\n", chainOutput, signalString);
        phaseNotSignal = !phaseNotSignal;
        return signalString;
    }
}

/**
 * This function chains programs together. Specifically, output from one is fed to input for two, etc.
 */ 
int chainProgram(int numChains, const char** inputs, int* program, int programLength, InputReader reader, OutputWriter writer) {
    //intialize 'global' storage for these programs
    int* programCopyStorage = malloc(programLength*sizeof(int));
    signalString = malloc(100*sizeof(char));

    chainOutput = 0;    //use the same mechanism for the first program as for the last. initialize to 0 for the first signal input.
    //run the program 'numChains' times
    for (int i=0; i<numChains; i++) {
        inputForChain = inputs[i];
        copyProgram(programCopyStorage, program, programLength);
        executeProgram(programCopyStorage, programLength, readInputForChain, writeOutputForChain);
    }

    //output the last output in the chain.
    writer(chainOutput);

    printf("final output = %i\n", chainOutput);
    free((void*)programCopyStorage);

    return chainOutput;
}

int decodeAmplifiers(int numAmplifiers, int* program, int programlength, InputReader reader, OutputWriter writer) {

    //will do this dynamically. just in case there are a different number of amplifiers in the future. also, i want to 
    //fiure out how to do this generically.
    int* permutationArray = malloc(numAmplifiers*sizeof(int));
    for (int i=0; i<numAmplifiers; i++) {
        permutationArray[i] = 0;
    }

    //elements in this array tell us when to toggle which elements in the array.
    //the algorithm is as follows: use the % operator for each element in the array. when the result is 0, that element needs
    //to be reset. **!!when that element is reset, the next index should be incremented.
    int* toggleThresholdArray = malloc(numAmplifiers*sizeof(int));
    int accumulator = numAmplifiers;
    int iterationCount = 1;
    for (int i=0; i<numAmplifiers; i++) {
        toggleThresholdArray[i]  = accumulator;
        accumulator*=numAmplifiers;
        iterationCount*=numAmplifiers;
    }    

    //just goingto hard-code a decent size for the storage of input strings.
    char** inputs = malloc(numAmplifiers*sizeof(char*));
    for (int i=0; i<numAmplifiers; i++) {
        char* input = malloc(100*sizeof(char));
        inputs[i] = input;
    }

    //the accumulator now holds the number of iterations we need to make to test all unique combinations
    int counter = 0;
    int maxSignal = -1;
    while (counter < iterationCount) {
        //generate input strings for program. NOTE: NO INPUT IS ALLOWED TO REPEAT. LET'S CHECK THAT HERE
        bool inputsMatch = false;
        for (int i=0; i<numAmplifiers; i++) {
            for (int j=0; j<numAmplifiers; j++) {
                int pi = permutationArray[i];
                int pj = permutationArray[j];
                if (i!=j && pi==pj) {
                    inputsMatch = true;
                }
            }
            sprintf(inputs[i], "%i", permutationArray[i]);
        }
        
        //don't do anything 
        if (!inputsMatch) {
            //run program
            int signal = chainProgram(numAmplifiers, (const char**)inputs, program, programlength, reader, writer);
            if (signal > maxSignal) {
                maxSignal = signal;
            }
        }
    
        counter++;

        //always increment least significant digit
        permutationArray[0]++;

        //increment permutations
        for (int i=0; i<numAmplifiers; i++) {
            int mod = toggleThresholdArray[i];
            if (counter%mod == 0 ) {
                //reset
                permutationArray[i] = 0;
                //next index needs to increment
                if (i<numAmplifiers-1) {
                    permutationArray[i+1]++;
                }
            }
        }

    }
    printf("max signal = %i\n", maxSignal);
    return maxSignal;

}