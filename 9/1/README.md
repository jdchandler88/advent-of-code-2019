# Day 9, part 1
We're augmenting IntCode again! Now we're adding relative addressing, increasing memory size, improving data handling, etc.

**NOTE!:** There isn't anything to do for part 2! We just had to supply a different input. Easy, since we already built it to take any input! 

# Build the code
```bash
make all
```
This will make both the executable and the test cases.

# Run the code
The following explains how to execute the code for both parts of day 7. It also describes the *positional* input arguments.

## Input arguments

1. inputFile: file containing IntCode program.
2. input: this is the input to the program


To run the code for the result for part 1:
```bash
./exe input.txt 1
```

To run the code for the result for part 2:
```bash
./exe input.txt 2
```

These examples will run with the input given by adventofcode. However, if you want to use your input, create a file and pass it to the executable.

# Run tests
```bash
./test
```
You **should** see no failures
