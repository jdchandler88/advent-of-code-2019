
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

int calculateMinIntersectionDistanceFromRouteStrings(const char* route1, const char* route2);

int calculateMinIntersectionDistance(struct Movement** route1, int route1Len, struct Movement** route2, int route2Len);

struct Movement** parseMovements(const char* route, int* numMovements);