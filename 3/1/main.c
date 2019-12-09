#include "wires.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

static void programStringCallback(const char* programString) {

}

int main(int argc, char** argv) {
    if (argc <= 1) {
        exit(0);
    }
    FILE* file = fopen(argv[1], "r");
    readLines(file, programStringCallback);
    fclose(file);
}