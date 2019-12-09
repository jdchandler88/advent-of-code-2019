# Day 2, part 2
This code determines the pair of inputs [1] (noun) and [2] (verb) that produce the output 19690720. This will use
the same input file as in the previous exercise.

There are no tests for this code. Technically, there should be since we're copying arrays, but we'll refactor to utils if 
necessary later and make tests. My rationale here is that we're using IntCode, which was already tested and proven to 
work in part 1.

# Build the code
```bash
make all
```
This will make both the executable and the test cases.

# Run the code
```bash
./exe ../1/input.txt $desiredOutput
```
This will run with the input given by adventofcode. However, if you want to use your input, create a file and pass it to the executable.
The desiredOutput should be passed into the exe as an argument. 
