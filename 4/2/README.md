# Day 4, part 2
This is the code to find all possible passwords within a range. Now there's an extra rule.
An adjacent group cannot be greater than 2 AND count toward satisfying the rule. If there is
another group that is size 2, then that will count. In other words, there must be at least one
group of exactly 2 consecutive matching digits.

# Build the code
```bash
make all
```
This will make both the executable and the test cases.

# Run the code
```bash
./exe input.txt
```
This will run with the input given by adventofcode. However, if you want to use your input, create a file and pass it to the executable.

# Run tests
```bash
./test
```
You **should** see no failures
