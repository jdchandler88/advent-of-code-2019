#include <stdbool.h>

/**
 * 
 **/
typedef enum OpCode {
  ADD = 1,
  MUL = 2,
  IN = 3,
  OUT = 4,
  JUMP_IF_TRUE = 5, 
  JUMP_IF_FALSE = 6, 
  LESS_THAN = 7,
  EQUALS = 8,
  HALT = 99
} OpCode;

typedef enum AddressingMode {
  INDIRECT = 0,
  DIRECT = 1
} AddressingMode;

/**
 * implementing parameters as linked list. Doing this becuase we don't know
 * how many there will be at runtime. it's easy to iterate and clean up, too.
 */
typedef struct Parameter {
  enum AddressingMode mode;
  struct Parameter* next;
} Parameter;

/**
 * Added on day7 part 2. This is because reading is becoming more complex.
 * I think a good way to do abstract reading is to implement a 'closure'.
 * 
 * The reader should return a const char* but how it does so is irrelevant. Also,
 * it may need an argument, or context. For 7/2, that context is a queue/thread.
 */ 
typedef struct InputReader {
  const char* (*reader)(void*);
  void* readerContext;
} InputReader;

/**
 * Also added on day7 part 2. Same reasoning as above.
 */ 
typedef struct OutputWriter {
  void (*writer)(int, void*);
  void* writerContext;
} OutputWriter;

/**
 * Added day7 part 2. Need to do this so we can pass arguments to a thread
 **/
typedef struct ProgramContext {
    int id;
    int* program;
    int programLength;
    InputReader* reader;
    OutputWriter* writer;
} ProgramContext;

const static int ADD_PARAMS = 3;
const static int MUL_PARAMS = 3;
const static int IN_PARAMS = 1;
const static int OUT_PARAMS = 1;
const static int JUMP_IF_TRUE_PARAMS = 2;
const static int JUMP_IF_FALSE_PARAMS = 2;
const static int LESS_THAN_PARAMS = 3;
const static int EQUAL_PARAMS = 3;
const static int HALT_PARAMS = 0;

/**
 * Each instruction consists of an opcode and a list of parameters.
 */
typedef struct Instruction {
  enum OpCode opcode;
  struct Parameter* parameter;
} Instruction;

struct Instruction* parseInstruction(int instructionString);

int decodeAmplifiers(int numAmplifiers, bool feedbackMode, int inputOffset, int* program, int programlength, InputReader* reader, OutputWriter* writer);

int chainProgram(int numChains, bool feedbackMode, const char** inputs, int inputOffset, int* program, int ProgramLength, InputReader* reader, OutputWriter* writer);

void executeProgram(struct ProgramContext* context);

void executeInstruction(int* program, int* programCounter, struct ProgramContext* ctx);