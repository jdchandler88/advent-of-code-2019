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