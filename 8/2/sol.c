#include "sol.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static struct Layer* createLayer(int width, int height) {
    struct Layer* layer = malloc(sizeof(Layer));
    layer->width = width;
    layer->height = height;
    layer->pixels = malloc(height*sizeof(char*));   //height = number of rows. create storage for each row.
    for (int row=0; row<=height; row++) {
        layer->pixels[row] = malloc(width*sizeof(char));    //create memory for enough characters in the row
    }
    return layer;
}

struct Image* parseImage(int width, int height, const char* input) {
    int lenData = strlen(input);
    int numLayers = lenData / (width*height);
    struct Image* image = malloc(sizeof(Image));
    //create storage for layers
    image->numLayers = numLayers;
    image->width = width;
    image->height = height;
    image->layers = malloc(numLayers*sizeof(Layer*));
    //create storage for each layer's data
    for (int i=0; i<numLayers; i++) {
        struct Layer* layer = createLayer(width, height);
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

//0 is black, 1 is white, and 2 is transparent.
struct Layer* composite(struct Image* image) {
    struct Layer* composite = createLayer(image->width, image->height);
    char pixelCopy[1];
    //iterate through pixels. we need to see value in each layer
    for (int rowIdx=0; rowIdx<image->height; rowIdx++) {
        for (int colIdx=0; colIdx<image->width; colIdx++) {
            //initialize composite pixel to transparent
            memcpy((char*)composite->pixels[rowIdx]+colIdx, "2", 1);
            //iterate through layers at the pixel to form composite
            for (int layerIdx=0; layerIdx<image->numLayers; layerIdx++) {
                struct Layer* currentLayer = image->layers[layerIdx];
                //get the int value of the character in the string
                memcpy(&pixelCopy, currentLayer->pixels[rowIdx]+colIdx, 1);
                int pixelValue = atoi(pixelCopy);
                //if it's an opaque color, then copy that into the composite
                if (pixelValue==0 || pixelValue ==1) {  //opaque color found. set this value in the composite
                    memcpy((char*)composite->pixels[rowIdx]+colIdx, currentLayer->pixels[rowIdx]+colIdx, 1);
                    break;
                }
            }
        }
    }
    return composite;
}

int pixelValue(struct Layer* layer, int row, int col) {
    char buf[1];
    memcpy(&buf, layer->pixels[row]+col, 1);
    return atoi(buf);
}

void printImage(struct Image* image) {
    struct Layer* compositeLayer = composite(image);

    for (int rowIdx=0; rowIdx<image->height; rowIdx++) {
        for (int colIdx=0; colIdx<image->width; colIdx++) {
            int pixel = pixelValue(compositeLayer, rowIdx, colIdx);
            const char* string;
            switch (pixel) {
                case 0:
                    string = "   ";
                    break;
                case 1:
                    string = "***";
                    break;
                case 2:
                    string = "XXX";
                    break;
                default:
                    string = "XXX";
                    break;
            }
            printf("%s", string);
        }
        printf("\n");
    }
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

int solutionForPart1(int width, int height, const char* input) {
    struct Image* image = parseImage(width, height, input);
    int layerIdx = findLayerWithLeastDigitCount(image, 0);  //find the layer with minimum number of zeros
    int sumOnes = sumDigitInLayer(1, image->layers[layerIdx]);
    int sumTwos = sumDigitInLayer(2, image->layers[layerIdx]);
    return sumOnes * sumTwos;
}

void solutionForPart2(int width, int height, const char* input) {
    struct Image* image = parseImage(width, height, input);
    printImage(image);
}