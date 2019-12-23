#include "sol.h"
#include "utils.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

program_t getValueFromMemoryAndIncrementProgramCounter(enum AddressingMode mode, struct ProgramContext* ctx) {
    //local variables to make typing easier...
    int* programCounter = &ctx->programCounter;
    program_t* program = ctx->program;
    program_t memoryElement = program[(*programCounter)++];
    program_t value;
    switch (mode) {
        case DIRECT:
            value = memoryElement;
            break;
        case INDIRECT:
            value = program[memoryElement];
            break;
        case RELATIVE:
            value = program[ctx->relativeBase + memoryElement];
            break;
    }
    return value;
}

void storeResultInMemoryAndIncrementProgramCounter(program_t value, enum AddressingMode mode, struct ProgramContext* ctx) {
    //local variables to make typing easier...
    int* programCounter = &ctx->programCounter;
    program_t* program = ctx->program;
    program_t location = program[(*programCounter)++];
    switch (mode) {
        case DIRECT:
            //does this ever happen? does it even make sense?
            break;
        case INDIRECT:
            program[location] = value;
            break;
        case RELATIVE:
            program[ctx->relativeBase + location] = value;
            break;
    }
}

static void handleAdd(struct Instruction* instruction, struct ProgramContext* ctx) {
    //local variables to make typing easier...
    int* programCounter = &ctx->programCounter;
    program_t* program = ctx->program;

    (*programCounter)++;
    program_t addend1 = getValueFromMemoryAndIncrementProgramCounter(instruction->parameter->mode, ctx);
    program_t addend2 = getValueFromMemoryAndIncrementProgramCounter(instruction->parameter->next->mode, ctx);
    program_t storeLocation = program[(*programCounter)++];
    program_t result = addend1 + addend2;
    program[storeLocation] = result;
};

static void handleMultiply(struct Instruction* instruction, struct ProgramContext* ctx) {
    //local variables to make typing easier...
    int* programCounter = &ctx->programCounter;
    program_t* program = ctx->program;
    
    (*programCounter)++;
    program_t addend1 = getValueFromMemoryAndIncrementProgramCounter(instruction->parameter->mode, ctx);
    program_t addend2 = getValueFromMemoryAndIncrementProgramCounter(instruction->parameter->next->mode, ctx);
    program_t storeLocation = program[(*programCounter)++];
    program_t result = addend1 * addend2;
    program[storeLocation] = result;
}

static void handleInput(struct Instruction* instruction, struct ProgramContext* ctx) {
    //local variables to make typing easier...
    int* programCounter = &ctx->programCounter;
    program_t* program = ctx->program;
    (*programCounter)++;
    const char* inputString = ctx->reader->reader(ctx->reader->readerContext);
    program_t input = atoi(inputString);
    storeResultInMemoryAndIncrementProgramCounter(input, instruction->parameter->mode, ctx);
}

static void handleOutput(struct Instruction* instruction, struct ProgramContext* ctx) {
    //local variables to make typing easier...
    int* programCounter = &ctx->programCounter;
    program_t* program = ctx->program;

    (*programCounter)++;
    //get value to output
    program_t output = getValueFromMemoryAndIncrementProgramCounter(instruction->parameter->mode, ctx);
    //write it to somewhere
    ctx->writer->writer(output, ctx->writer->writerContext);
}

static void handleJumpIfTrue(struct Instruction* instruction, struct ProgramContext* ctx) {
    //local variables to make typing easier...
    int* programCounter = &ctx->programCounter;
    program_t* program = ctx->program;

    (*programCounter)++;
    program_t value1 = getValueFromMemoryAndIncrementProgramCounter(instruction->parameter->mode, ctx);
    program_t jumpLocation = getValueFromMemoryAndIncrementProgramCounter(instruction->parameter->next->mode, ctx);
    if (value1!=0) {
        *programCounter = jumpLocation;
    }
}

static void handleJumpIfFalse(struct Instruction* instruction, struct ProgramContext* ctx) {
    //local variables to make typing easier...
    int* programCounter = &ctx->programCounter;
    program_t* program = ctx->program;
    
    (*programCounter)++;
    program_t value1 = getValueFromMemoryAndIncrementProgramCounter(instruction->parameter->mode, ctx);
    program_t jumpLocation = getValueFromMemoryAndIncrementProgramCounter(instruction->parameter->next->mode, ctx);
    if (value1==0) {
        *programCounter = jumpLocation;
    }
}

static void handleLessThan(struct Instruction* instruction, struct ProgramContext* ctx) {
    //local variables to make typing easier...
    int* programCounter = &ctx->programCounter;
    program_t* program = ctx->program;

    (*programCounter)++;
    program_t value1 = getValueFromMemoryAndIncrementProgramCounter(instruction->parameter->mode, ctx);
    program_t value2 = getValueFromMemoryAndIncrementProgramCounter(instruction->parameter->next->mode, ctx);
    program_t storeLocation = program[(*programCounter)++];
    program[storeLocation] = (value1<value2);
}

static void handleEquals(struct Instruction* instruction, struct ProgramContext* ctx) {
    //local variables to make typing easier...
    int* programCounter = &ctx->programCounter;
    program_t* program = ctx->program;

    (*programCounter)++;
    program_t value1 = getValueFromMemoryAndIncrementProgramCounter(instruction->parameter->mode, ctx);
    program_t value2 = getValueFromMemoryAndIncrementProgramCounter(instruction->parameter->next->mode, ctx);
    program_t storeLocation = program[(*programCounter)++];
    program[storeLocation] = (value1==value2);
}

static void handleAdjustRelativeBase(struct Instruction* instruction, struct ProgramContext* ctx) {
    //local variables to make typing easier...
    int* programCounter = &ctx->programCounter;
    program_t* program = ctx->program;

    (*programCounter)++;
    program_t adjustValue = getValueFromMemoryAndIncrementProgramCounter(instruction->parameter->mode, ctx);
    ctx->relativeBase += adjustValue;
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
        case ADJUST_RELATIVE_BASE: 
            return ADJUST_RELATIVE_BASE_PARAMS;
        default:
            printf("unexpected opcode in parse. quitting.\n");
            exit(1);
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
struct Instruction* parseInstruction(program_t instructionInt) {
    program_t intMask = 100;
    program_t tmp = instructionInt%intMask;
    program_t opcode = tmp;
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
        
        int mode = tmp/(intMask/10);
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

static void printIntArray(program_t* arr, int size) {
    for (int i=0; i<size; i++) {
        if (i==0) {
            printf("[");
        }
        printf("%ld", arr[i]);
        if (i!=size-1) {
            printf(", ");
        } else {
            printf("]");
        }
    }
    printf("\n");
}

void executeProgram(struct ProgramContext* context) {
    while (context->program[context->programCounter] != HALT) {
        executeInstruction(context);
    }
    // printf("executed program. result = ");
    // printIntArray(context->program, context->programLength);
}

struct Parameter* nextParameter(struct Parameter* currentParameter) {
    return currentParameter->next;
}

void executeInstruction(struct ProgramContext* ctx) {
    int pc = ctx->programCounter;
    struct Instruction* instruction = parseInstruction(ctx->program[pc]);
    switch (instruction->opcode) {
        case ADD:
            handleAdd(instruction, ctx);
            break;
        case MUL:
            handleMultiply(instruction, ctx);
            break;
        case  IN:
            handleInput(instruction, ctx);
            break;
        case JUMP_IF_TRUE:
            handleJumpIfTrue(instruction, ctx);
            break;
        case JUMP_IF_FALSE:
            handleJumpIfFalse(instruction, ctx);
            break;
        case LESS_THAN:
            handleLessThan(instruction, ctx);
            break;
        case EQUALS:
            handleEquals(instruction, ctx);
            break;
        case OUT:
            handleOutput(instruction, ctx);
            break;
        case ADJUST_RELATIVE_BASE: 
            handleAdjustRelativeBase(instruction, ctx);
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

static void copyProgram(program_t* storageLocation, program_t* program, int programLength) {
   memcpy(storageLocation, program, programLength * sizeof(program_t));
}

typedef struct QueueContext {
    int id;
    struct Queue* queue;
} QueueContext;

//reads from a queue in a thread-safe manner. 
//also, if there is no input available, then the current thread waits until it's signaled that there is input available
const char* readQueue(void* context) {
    struct QueueContext* ctx = (QueueContext*)context;
    char* input = malloc(100*sizeof(char));
    void* queueValue = queue_dequeue(ctx->queue).element.anyPointer;
    program_t value = *((program_t*)queueValue);
    sprintf(input, "%ld", value);
    return input;
}

//writes to queue in thread-safe manner.
//this also notifies a wait condition in case another thread is waiting
void writeQueue(program_t output, void* context) {
    struct QueueContext* ctx = (QueueContext*)context;
    //create storage for program_t, copy it to pointer, and then use that pointer for queue output
    void* outputForQueue = malloc(sizeof(program_t));
    memcpy(outputForQueue, &output, sizeof(program_t));
    struct QueueElement outputElement = {VOIDP, {.anyPointer=outputForQueue}};
    //put data in queue and notify wait condition just in case other threads are waiting for input
    queue_enqueue(ctx->queue, outputElement);
}

/**
 * This function chains programs together. Specifically, output from one is fed to input for two, etc.
 */ 
int chainProgram(int numChains, bool feedbackMode, const char** inputs, int inputOffset, program_t* program, int programLength, InputReader* reader, OutputWriter* writer) {
    bool sequentialNotParallel = !feedbackMode;  //if true, then execute programs sequentially. otherwise, execute them in parallel

    //queues for communication between units
    struct QueueContext** ctxs = malloc(numChains*sizeof(QueueContext*));
    for (int i=0; i<numChains; i++) {
        struct QueueContext* ctx = malloc(sizeof(QueueContext));
        ctx->id = i;
        ctx->queue = queue_create();
        ctxs[i] = ctx;

        //initialize inputs for each component. each component reads input from the previous, so set the queues up that way
        int inputIdx;;
        if (i==numChains-1) {
            inputIdx = 0;
        } else {
            inputIdx = i+1;
        }
        writeQueue(atoi(inputs[inputIdx]), ctx);
    }           

    //we need to initialize input for the first unit
    struct QueueContext* firstContext = ctxs[numChains-1];
    writeQueue(0, firstContext);


    pthread_t threads[numChains];

    //there are #units queues
    for (int i=0; i<numChains; i++) {
        //create program context
        struct ProgramContext* programContext = malloc(sizeof(ProgramContext));

        //make copy of program and store in context
        program_t* programCopy = calloc(programLength + 1000, sizeof(program_t));
        copyProgram(programCopy, program, programLength);
        programContext->id = i;
        programContext->programCounter = 0;
        programContext->program = programCopy;
        programContext->programLength = programLength;
        programContext->reader = malloc(sizeof(InputReader));
        programContext->reader->reader = readQueue;
        programContext->reader->readerContext = (i==0) ? ctxs[numChains-1] : ctxs[i-1]; //if this is the first unit, read from *the last queue*; this is the queue to which the last unit writes. otherwise, read from the one previous
        programContext->writer = malloc(sizeof(OutputWriter));
        programContext->writer->writer = writeQueue;
        programContext->writer->writerContext= ctxs[i];    //always write to the queue ot which the unit is assigned (the one in front of the unit);
        //execute program in another thread
        pthread_create(&threads[i], NULL, (void* (*)(void*))executeProgram, programContext);

        //if sequential, then join each individual thread here (each component will complete prior to the proceeding component)
        if (sequentialNotParallel) {
            pthread_join(threads[i], NULL);
        }
    }

    //if parallel, join all threads now
    if (!sequentialNotParallel) {
        for (int i=0; i<numChains; i++) {
            pthread_join(threads[i], NULL);
        }
    }

    //output the last output in the chain.
    program_t value = *(program_t*)queue_dequeue(ctxs[numChains-1]->queue).element.anyPointer;
    writer->writer(value, writer->writerContext);

    // free((void*)programCopyStorage);

    return value;
}

int decodeAmplifiers(int numAmplifiers, bool feedbackMode, int inputOffset, program_t* program, int programlength, InputReader* reader, OutputWriter* writer) {

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
            //adjust inputs for offset
            sprintf(inputs[i], "%i", permutationArray[i] + inputOffset);
        }
        
        //only run the program if none of the inputs match
        if (!inputsMatch) {
            //run program
            int signal = chainProgram(numAmplifiers, feedbackMode, (const char**)inputs, inputOffset, program, programlength, reader, writer);
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