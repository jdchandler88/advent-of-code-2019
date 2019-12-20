typedef struct Layer {
    const char** pixels;
} Layer;

typedef struct Image {
    int width;
    int height;
    int numLayers;
    struct Layer** layers;
} Image;

struct Image* createImage(int width, int height, const char* input);