#include "sol.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void programStringCallback(const char* programString) {
    
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("gimme an input file, number of amplifiers we're decoding, feedbackMode, inputOffset. For example './exe input.txt 5 1 0'\n");
        printf("\tinputFile should be a string pointing to a file\n");
        printf("\tinput. this is used to pass to the BOOST program\n");
        exit(0);
    }
}