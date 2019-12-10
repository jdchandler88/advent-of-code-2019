#include "utils.h"
#include <stdlib.h>
#include <string.h>

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

const char** parseString(const char* string, const char* delimiters, int* numberStringsParsed) {
    char* stringCopy = (char*)copyString(string);

    //storage into which parsed tokens are placed
    const char** tokens = malloc(sizeof(char*));
    char *token;
    token = strtok(stringCopy, delimiters);

    //store first token
    int tokensSize = 0;

    /* walk through other tokens */
    while( token != NULL ) 
    {   
        //if not on teh first loop, let's make more space for the next token
        tokensSize++;
        if (tokensSize > 1) {
            tokens = realloc(tokens, tokensSize * sizeof(char*));
        }
        //copy the token so we can use it later. otherwise, strtok will blast the 'token' ptr for the next loop
        const char* tokenCopy = copyString(token);
        tokens[tokensSize-1] = tokenCopy;
        //tokenize for next loop iteration
        token = strtok(NULL, delimiters);
    }
    //we're finished parsing the string. delete the bufferspace
    free((void*)stringCopy);

    //return values
    *numberStringsParsed = tokensSize;
    return tokens;
}

const char* copyString(const char* string) {
    int length = strlen(string);
    char* copy = malloc( (length * sizeof(char)) + 1);
    strcpy(copy, string);
    return copy;
}

void freeArray(void** array, int size) {
    for (int i=0; i<size; i++) {
        free(array[i]);
    }
    free(array);
}

void freeLinkedList(void* node, void* (*next)(void*)) {
    void* previousNode = node;
    void* currentNode = next(previousNode);
    while (currentNode != NULL) {
        free((void*)previousNode);
        previousNode = currentNode;
        currentNode = next(currentNode);
    }
}