#include "password.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

static void programStringCallback(const char* line) {
    testPasswordRangeString(line);
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("bruh. gimme an input file.\n");
        exit(0);
    }
    FILE* file = fopen(argv[1], "r");
    readLines(file, programStringCallback);
    fclose(file);
}