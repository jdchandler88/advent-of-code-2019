#include "utils.h"
#include <stdlib.h>

void readLines(FILE* file, void (*lineCallback)(const char*)) {
    char * line = NULL;
    size_t len = 0;
    size_t read;

    if (file == NULL)
        exit(EXIT_FAILURE);
    //notify listener when line is read
    while ((read = getline(&line, &len, file)) != -1) {
        (*lineCallback)(line);
    }
    //multiple calls to getline reallocate the buffer if it is not big enough. in other words, 
    //a single buffer is used. only free after finished reading the frile
    free(line);
}