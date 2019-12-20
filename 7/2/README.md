# Day 7, part 2
We're using the latest IntCode to solve a problem. We're still not modifying IntCode. This time we're organizing the units
in a feedback loop AND we need to run them in parallel. I'm going to build this such that flags will run it either way.

# Build the code
```bash
make all
```
This will make both the executable and the test cases.

# Run the code
The following explains how to execute the code for both parts of day 7. It also describes the *positional* input arguments.

## Input arguments

1. inputFile: file containing IntCode program.
2. numAmps: integer representing the number of amplifiers in the chain
3. feedbackMode: integer representing whether or not the amp are configured in feedback mode. '1' for feedback loop (parallel execution), '0' for no feedback (sequential execution)
4. inputOffset: This program is built to calculate all input permutations given the requirements. These permutations are 0-based. The input offset simply adds
this value to each permutation element.


To run the code for the result for part 1:
```bash
./exe input.txt 5 0 0
```

To run the code for the result for part 2:
```bash
./exe input.txt 5 1 5
```

These examples will run with the input given by adventofcode. However, if you want to use your input, create a file and pass it to the executable.

# Run tests
```bash
./test
```
You **should** see no failures
