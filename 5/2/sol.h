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

typedef const char* (*InputReader)(void);
typedef void (*OutputWriter)(int);

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

void executeProgram(int* program, int programLength, InputReader reader, OutputWriter writer);

void executeInstruction(int* program, int* programCounter);