#include "sol.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

const char** mappings = NULL;
int curSize = 1;

static void lineCallback(const char* string) {
    if (mappings == NULL) {
        mappings = malloc(sizeof(const char*));
    } else {
        curSize++;
        mappings = realloc(mappings, curSize*sizeof(const char*));
    }
    mappings[curSize-1] = copyString(string);
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        exit(0);
    }
    FILE* file = fopen(argv[1], "r");
    readLines(file, lineCallback);
    countOrbits(mappings, curSize);
    //freeArray((void**)mappings, curSize);
    fclose(file);
}