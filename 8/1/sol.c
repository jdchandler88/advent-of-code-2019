#include "sol.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Image* parseImage(int width, int height, const char* input) {
    int lenData = strlen(input);
    int numLayers = lenData / (width*height);
    struct Image* image = malloc(sizeof(Image));
    //create storage for layers
    image->numLayers = numLayers;
    image->layers = malloc(numLayers*sizeof(Layer*));
    //create storage for each layer's data
    for (int i=0; i<numLayers; i++) {
        struct Layer* layer = malloc(sizeof(Layer));
        layer->width = width;
        layer->height = height;
        layer->pixels = malloc(height*sizeof(char*));   //height = number of rows. create storage for each row.
        for (int row=0; row<=height; row++) {
            layer->pixels[row] = malloc(width*sizeof(char));    //create memory for enough characters in the row
        }
        image->layers[i] = layer;
    }

    //copy values into image storage location
    int cursor = 0;
    for (int layerIdx=0; layerIdx<numLayers; layerIdx++) {
        struct Layer* layer = image->layers[layerIdx];
        for (int row=0; row<height; row++) {
            memcpy((char*)layer->pixels[row], input+cursor, width);
            cursor+=width;
        }
    }

    return image;
} 

int sumDigitInLayer(int digit, struct Layer* layer) {
    int sum = 0;
    char buf[1];
    for (int rowIdx=0; rowIdx<layer->height; rowIdx++) {
        const char* row = layer->pixels[rowIdx];
        for (int column=0; column<layer->width; column++) {
            memcpy(&buf, row+column, 1);
            if (atoi(buf) == digit) {
                sum++;
            }
        }
    }
    return sum;
}

int findLayerWithLeastDigitCount(struct Image* image, int digit) {
    int min = __INT_MAX__;
    int minLayer = -1;
    for (int layerIdx=0; layerIdx<image->numLayers; layerIdx++) {
        int sumOfDigits = sumDigitInLayer(digit, image->layers[layerIdx]);
        if (sumOfDigits < min) {
            min = sumOfDigits; 
            minLayer = layerIdx;
        }
    }
    return minLayer;
}