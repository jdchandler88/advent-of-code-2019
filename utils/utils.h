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

typedef struct Queue Queue;

/**
 * Allows storing of "any" type in queue
 **/
typedef struct QueueElement {
    enum ElementType {
        SHORT,
        INT,
        LONG,
        LONG_LONG,
        FLOAT,
        DOUBLE,
        // LONG_DOUBLE, //got this note from my compiler: note: the ABI of passing union with long double has changed in GCC 4.4
        VOIDP
    } elementType;
    union Element {
        short s;
        int i;
        long l;
        long long ll;
        float f;
        double d;
        // long double ld;
        void* anyPointer;
    } element;
} QueueElement;

struct Queue* queue_create();

void queue_destroy(struct Queue* queue);

void queue_enqueue(struct Queue* queue, struct QueueElement queueElement);

struct QueueElement queue_dequeue(struct Queue* queue);

int queue_size(struct Queue* queue);

static int increaseQueueSize(struct Queue* queue, int growthSize);