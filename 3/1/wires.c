#include "wires.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

static void freeMovementsMemory(struct Movement** movements, int sizeofRoute) {
    for (int i=0; i<sizeofRoute; i++) {
        free((void*)movements[i]);
    }
    free((void*)movements);
}

int calculateMinIntersectionDistanceFromRouteStrings(const char* route1String, const char* route2String) {
    int sizeofRoute1;
    struct Movement** route1 = parseMovements(route1String, &sizeofRoute1);
    int sizeofRoute2;
    struct Movement** route2 = parseMovements(route2String, &sizeofRoute2);
    int minDistance = calculateMinIntersectionDistance(route1, sizeofRoute1, route2, sizeofRoute2);
    //free memory
    freeMovementsMemory(route1, sizeofRoute1);
    freeMovementsMemory(route2, sizeofRoute2);
    //give dat answer doe
    return minDistance;
}

int calculateMinIntersectionDistance(struct Movement** route1, int route1Len, struct Movement** route2, int route2Len) {
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
    Can do some simple linear algebra to determine intersection points. Probs should go that route.  For instance, a vertical line with origin (0,0) 
    has a certain equation. The intersection with a segment of a different wire can easily be calculated. We'll need to limit intersection logic
    with the distance of that segment (i.e. lines are not infinite)
    */
    int x=0;
    int y=0;
    for (int i=0; i<route1Len; i++) {
        struct Movement* movement = route1[i];
        switch (movement->direction) {
            case UP:
                y += movement->numMoves;
                break;
            case DOWN:
                y -= movement->numMoves;
                break;
            case LEFT:
                x -= movement->numMoves;
                break;
            case RIGHT:
                x += movement->numMoves;
                break;
        }
        // if (x<0 || y <=0) {
            printf("(%i,%i)\n", x, y);
        // }
    };

    return -1;
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