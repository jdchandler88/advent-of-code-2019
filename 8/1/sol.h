typedef struct Layer {
    int width; 
    int height;
    const char** pixels;
} Layer;

typedef struct Image {
    int width;
    int height;
    int numLayers;
    struct Layer** layers;
} Image;

struct Image* parseImage(int width, int height, const char* input);

int sumDigitInLayer(int digit, struct Layer* layer);

int findLayerWithLeastDigitCount(struct Image* image, int digit);

int solutionForPart1(int width, int height, const char* input);