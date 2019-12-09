/**
 * 
 **/
typedef enum OpCode {
  ADD = 1,
  MUL = 2,
  HALT = 99
} OpCode;

void executeProgram(int* program, int programLength);

void executeInstruction(int* program, int* programCounter);