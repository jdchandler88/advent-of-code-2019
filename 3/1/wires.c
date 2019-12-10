#include "wires.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

int calculateMinIntersectionDistanceFromRouteStrings(const char* route1String, const char* route2String) {
    int sizeofRoute1;
    struct Movement** route1 = parseMovements(route1String, &sizeofRoute1);
    int sizeofRoute2;
    struct Movement** route2 = parseMovements(route2String, &sizeofRoute2);
    int minDistance = calculateMinIntersectionDistance(route1, sizeofRoute1, route2, sizeofRoute2);
    //free memory
    freeArray((void**)route1, sizeofRoute1);
    freeArray((void**)route2, sizeofRoute2);
    //give dat answer doe
    return minDistance;
}

struct Point* nextPointInDirection(struct Point* p0, enum Direction direction) {
    struct Point* next = malloc(sizeof(struct Point));
    next->x = p0->x;
    next->y = p0->y;
    next->next = NULL;
    switch(direction) {
        case UP:
            next->y++;
            break;
        case DOWN:
            next->y--;
            break;
        case LEFT:
            next->x--;
            break;
        case RIGHT:
            next->x++;
            break;
    }
    p0->next = next;
    return next;
}

struct Point* moveToNextPoint(struct Point* p0, struct Movement* movement) {
    struct Point* currentPoint = p0;
    for (int i=0; i<movement->numMoves; i++) {
        struct Point* next = nextPointInDirection(currentPoint, movement->direction);
        currentPoint = next;
    }
    return currentPoint;
}

struct Point* mapRoute(struct Movement** route, int routeLen) {
    struct Point* firstPoint = malloc(sizeof(struct Point));
    firstPoint->x = 0;
    firstPoint->y = 0;

    int x=0;
    int y=0;
    struct Point* currentPoint = firstPoint;
    for (int i=0; i<routeLen; i++) {
        currentPoint = moveToNextPoint(currentPoint, route[i]);
    };
    return firstPoint;
}

struct Point* nextPoint(struct Point* point) {
    return point->next;
}

/*
Seems like there are some options if you're going to go the route of storing information about a wire's location. 
The general algorithm i'll use is: plot out the first wire in memory. Then, follow the path of the second wire, 
checking at each location in the grid if it exists for the other wire.

A hashmap would be a good storage mechanism. We would store an associative array of coordinates that
exist for a wire. Then, when mapping the second wire, we can simply check existence of a coordinate in that
storage to see if there is an intersection.

A linked list could work, too. Only problem is we need to possibly iterate over every element to see if the first
wire has a location equal to the second. A linked list node would contain an x, y, and a pointer to the next node.

A 2D array would work as well. Grow the array as large as necessary. For instance, make the array as large as necessary
for the first direction. Then, if more directions make it need to grow, then grow it. Buuut, what if it needs to grow *backwards?*
How to handle that? Could have two arrays, one for 0+ and one for -.


*************

After thinking about it some more, a mroe  efficient approach would be to calculate a line from each segment and see if those lines intersect.
Can do some simple linear algebra to determine intersection points. For instance, a vertical line with origin (0,0) 
has a certain equation. The intersection with a segment of a different wire can easily be calculated. We'll need to limit intersection logic
with the distance of that segment (i.e. lines are not infinite)

*************

Going with the linked list. Just the easiest to implement.

*/
int calculateMinIntersectionDistance(struct Movement** route1, int route1Len, struct Movement** route2, int route2Len) {
    //map routes using movements
    struct Point* route1FirstPoint = mapRoute(route1, route1Len);
    struct Point* route2FirstPoint = mapRoute(route2, route2Len);

    int minDistance = __INT_MAX__;
    //for every point in route 2, see if it exists in route 1. if so, then calculate the distance
    struct Point* currentRoute2Point = route2FirstPoint;
    while (currentRoute2Point != NULL) {
        struct Point* currentRoute1Point = route1FirstPoint;
        while (currentRoute1Point != NULL) {
            //check points are same BUT ALSO NOT AT ORIGIN!
            if (currentRoute2Point->x == currentRoute1Point->x && currentRoute2Point->y == currentRoute1Point->y && !(currentRoute2Point->x == 0 && currentRoute2Point->y == 0) ) {
                int distance = abs(currentRoute2Point->x) + abs(currentRoute2Point->y);
                printf("Found intersection at: (%i,%i)\n", currentRoute2Point->x, currentRoute2Point->y);
                if (distance < minDistance) {
                    minDistance = distance;
                }
            }
            currentRoute1Point = currentRoute1Point->next;
        }
        currentRoute2Point = currentRoute2Point->next;
    }

    // //free up memory
    freeLinkedList((void*)route1FirstPoint, (void*(*)(void*))nextPoint);
    freeLinkedList((void*)route2FirstPoint, (void*(*)(void*))nextPoint);

    //dat result doe
    return minDistance;
}

struct Movement** parseMovements(const char* route, int* numMovements) {
    int sizeofRoute = 0;
    const char** movementStrings = parseString(route, ",", &sizeofRoute);
    struct Movement** movements = malloc(sizeofRoute*(sizeof (Movement*)));
    
    for (int i=0; i<sizeofRoute; i++) {
        const char* movementString = movementStrings[i];
        //parse direction component of movement
        Direction dir;
        switch (movementString[0]) {
            case 'U':
                dir = UP;
                break;
            case 'D':
                dir = DOWN;
                break;
            case 'L':
                dir = LEFT;
                break;
            case 'R':
                dir = RIGHT;
                break;
        }
        //parse numMoves component
        int movementStringLength = strlen(movementString); //keep this number. normally, we'd add one for null terminator. however, we're taking one off for the direction component
        char numMovesString[movementStringLength];
        memcpy(numMovesString, movementString+1, movementStringLength);
        int numMoves = atoi(numMovesString);

        //create struct and insert it in array
        struct Movement* movement = malloc(sizeof(Movement));
        movement->direction = dir;
        movement->numMoves = numMoves;
        movements[i] = movement;
    }
    //return values
    *numMovements = sizeofRoute;
    return movements;
}