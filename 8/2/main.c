#include "sol.h"
#include <stdio.h>
#include <math.h>
#include <utils.h>
#include <stdlib.h>

int width;
int height;

void lineCallback(const char* line) {
    printf("solution for part2 below...\n");
    solutionForPart2(width, height, line);
}

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Arguments I need are: file width height. See README for more details.");
        return 1;   //no file provided
    }
    //just assume a file. we would be robust if this were for production
    FILE* f = fopen(argv[1], "r");
    width = atoi(argv[2]);
    height = atoi(argv[3]);
    readLines(f, lineCallback);
    fclose(f);
    return 0;
}