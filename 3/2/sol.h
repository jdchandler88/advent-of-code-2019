
typedef enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct Movement {
    enum Direction direction;
    int numMoves;
} Movement;

typedef struct Point {
    int x;
    int y;
    struct Point* next;
} Point;

int calculateMinIntersectionStepsFromRouteStrings(const char* route1, const char* route2);

int calculateMinIntersectionSteps(struct Movement** route1, int route1Len, struct Movement** route2, int route2Len);

struct Movement** parseMovements(const char* route, int* numMovements);