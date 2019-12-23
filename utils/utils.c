#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

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

struct Queue {
  int id;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  struct QueueElement* queue;
  int numQueued;
  int maxQueueSize;
};

static const int QUEUE_GROWTH_SIZE=10;
static int idCounter = 0;

struct Queue* queue_create() {
    struct Queue* queue = malloc(sizeof(struct Queue));
    queue->id = idCounter++;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);
    queue->numQueued = 0;
    queue->maxQueueSize = QUEUE_GROWTH_SIZE;
    queue->queue = malloc(queue->maxQueueSize*sizeof(QueueElement));
    return queue;    
}

void queue_destroy(struct Queue* queue) {
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);
    free(queue->queue);
    free(queue);
}

void queue_enqueue(struct Queue* queue, struct QueueElement value) {
    pthread_mutex_lock(&queue->mutex);
    //is there enough room? if so, add it. if not, reize it
    if (queue->maxQueueSize == queue->numQueued) {
        increaseQueueSize(queue, QUEUE_GROWTH_SIZE);
    }
    //copy value into queue memory
    memcpy(&queue->queue[queue->numQueued], &value, sizeof(QueueElement));
    queue->numQueued++;
    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

struct QueueElement queue_dequeue(struct Queue* queue) {
    pthread_mutex_lock(&queue->mutex);
    //is there anything in the queue? if so, then get it. if not, then suspend until there is something available.
    if (queue->numQueued == 0) {
        //wait for write to queue
        pthread_cond_wait(&queue->cond, &queue->mutex);
    } 
    queue->numQueued--;
    //get value, copy it for return, and free the original value
    struct QueueElement value=queue->queue[0];
    //shift values up (a more sophisticated implementation would use a circular pointer or something...)
    for (int i=0; i<queue->numQueued; i++) {
        queue->queue[i] = queue->queue[i+1];
    }
    pthread_mutex_unlock(&queue->mutex);
    return value;
}

int queue_size(struct Queue* queue) {
    pthread_mutex_lock(&queue->mutex);
    int value = queue->numQueued;
    pthread_mutex_unlock(&queue->mutex);
    return value;
}

static int increaseQueueSize(struct Queue* queue, int growthSize) {
    queue->maxQueueSize+=growthSize;
    queue->queue = realloc(queue->queue, queue->maxQueueSize*sizeof(QueueElement));
}