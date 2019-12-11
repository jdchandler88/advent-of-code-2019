# Day 1, part 2
This version of IntCode adds input/output instructions and introduces the immediate addressing mode.

# Build the code
```bash
make all
```
This will make both the executable and the test cases.

# Run the code
```bash
./exe input.txt YOU SAN
```
This will run with the input given by adventofcode. However, if you want to use your input, create a file and pass it to the executable.

Rather than hard-coding YOU and SAN in teh code, you can find any number of orbital transfers. I didn't make this super robust, so you may find
edge cases that blow this up.

# Run tests
```bash
./test
```
You **should** see no failures
