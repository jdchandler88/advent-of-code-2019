#include <stdio.h>

/**
 * Read a file line-by-line and call the lineCallback(const char*) 
 * function for every line
 **/
void readLines(FILE* file, void (*lineCallback)(const char*));

/**
 * Parse a string into tokens using provided delimiters. The number of strings parsed will
 * be set in the 'numberStringsParsed' pointer
 **/
const char** parseString(const char* string, const char* delimiters, int* numberStringsParsed);

/**
 * copy string
 **/
const char* copyString(const char* string);