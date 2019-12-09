#include <stdio.h>

/**
 * Read a file line-by-line and call the lineCallback(const char*) 
 * function for every line
 **/
void readLines(FILE* file, void (*lineCallback)(const char*));

/**
 * Parse a string into tokens using provided delimiters
 **/
const char** parseString(const char* string, const char* delimiters);

/**
 * copy string
 **/
const char* copyString(const char* string);