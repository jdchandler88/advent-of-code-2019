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

/**
 * frees array of pointers and the array storage itself
 **/ 
void freeArray(void** array, int numElements);

/**
 * frees linked list. the second argument is a function pointer that should return the next
 * element in the linked list.
 **/
void freeLinkedList(void* node, void* (*next)(void*));