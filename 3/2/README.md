# Day 3, part 2
Part 1 requirement is augmented such that the signal delay must be minimized. In other words, the least *combined number of steps for **each wire*** 
must be calculated. The minimum of that number should be returned.

In retrospect, we're sort of rewarded for going a brute-force approach in mapping routes in this second part. Imagine I went with the solution
for line equations and calculating intersections that way. There would be no mechanism in place for iterating over the path. I just had to make 
slight modifications for the approach I chose. Lucky!

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
