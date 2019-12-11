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
    if (argc != 4) {
        printf("Expected arguments: FILE, START-PLANET, END-PLANET (in that order)\n");
        exit(0);
    }
    FILE* file = fopen(argv[1], "r");
    const char* start = argv[2];
    const char* end = argv[3];
    readLines(file, lineCallback);
    countRequiredTransfers(start, end, mappings, curSize);
    //freeArray((void**)mappings, curSize);
    fclose(file);
}