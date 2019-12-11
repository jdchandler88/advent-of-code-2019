#include "sol.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

const char* sanitizeStringOfNewline(const char* string) {
    // char* replace_char(char* str, char find, char replace){
    char* str = (char*)copyString(string);
    char find = '\n';
    char replace = '\0';
    char *current_pos = strchr(str,find);
    while (current_pos){
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
    return str;
// }
}

struct Planet* searchForPlanet(struct Planet** planets, int numPlanets, const char* name) {
    for (int i=0; i<numPlanets; i++) {
        //check planet
        struct Planet* planet = planets[i];

        //we won't necessarily have a fully-populated list of planets. short-circuit if we hit the end of the list
        if (planet == NULL) {
            return NULL;
        }

        if (strcmp(name, planet->name) == 0) {
            return planet;
        }
        //check planet's children
        struct Planet* childPlanetMatch = searchForPlanet(planet->children, planet->numChildren, name);
        if (childPlanetMatch != NULL) {
            return childPlanetMatch;
        }
    }
    return NULL;
}

int getNextPlanetStorageIndex(struct Planet** planets, int numPlanets) {
    for (int i=0; i<numPlanets; i++) {
        if (planets[i] == NULL) {
            return i;
        }
    }
    return -1;
}

struct Planet* createPlanet(const char* name) {
    struct Planet* planet = malloc(sizeof(struct Planet));
    planet->name = name;
    planet->parent = NULL;
    planet->children = NULL;
    planet->numChildren = 0;
    return planet;
}

void addChildToParent(struct Planet** planets, int numPlanets, struct Planet* parent, const char* childName) {
    //see if it has already been added to global storage. if not, then create it
    struct Planet* child = searchForPlanet(planets, numPlanets, childName);
    if (child == NULL) {
        child = createPlanet(childName);
    }
    //initialize child
    child->parent = parent;
    parent->numChildren++;
    if (parent->children == NULL) {
        parent->children = malloc(sizeof(struct Planet*));
    } else {
        parent->children = realloc(parent->children, parent->numChildren*sizeof(struct Planet*));
    }
    parent->children[parent->numChildren-1] = child;
}

void printAllPlanets(struct Planet* root, int* indentLevel) {
    for (int i=0; i<*indentLevel; i++) {
        printf(" ");
    }
    printf("planet = %s, numChildren = %i\n", root->name, root->numChildren);
    (*indentLevel)++;
    for (int i=0; i<root->numChildren; i++) {
        printAllPlanets(root->children[i], indentLevel);
    }
    (*indentLevel)--;
}

void countAllOrbits(struct Planet* root, int* count, int* sum) {
    for (int i=0; i<root->numChildren; i++) {
        (*count)++;
        (*sum)=*sum+*count;
        countAllOrbits(root->children[i], count, sum);
        (*count)--;
    }
}

static struct Planet** mapPlanets(const char** mappings, int numMappings) {
    //create storage for all planets. We'll reorganize after they're all created. initialize to NULL
    struct Planet** planets = malloc(numMappings*sizeof(struct Planet));
    for (int i=0; i<numMappings; i++) {
        planets[i] = NULL;
    }

    //start mapping the orbits
    for (int i=0; i<numMappings; i++) {
        //parse the mappings
        const char* mapping = mappings[i];
        int numTokens;
        //planets[0] = parent
        //planets[1] = child
        const char** planetsStrings = parseString(mapping, ")", &numTokens);
        const char* parentName = planetsStrings[0];
        const char* childName = sanitizeStringOfNewline(planetsStrings[1]);

        //check to see if parent exists.
        struct Planet* parent = searchForPlanet(planets, numMappings, parentName);
        if (parent == NULL) {
            parent = createPlanet(parentName);
            //add parent to storage
            int planetStorageIndex = getNextPlanetStorageIndex(planets, numMappings);
            planets[planetStorageIndex] = parent;
        }
        addChildToParent(planets, numMappings, parent, childName);
    }
    return planets;
}

int countOrbits(const char** mappings, int numMappings) {
    
    struct Planet** planets = mapPlanets(mappings, numMappings);

    struct Planet* com = searchForPlanet(planets, numMappings, "COM");
    // debugging: can see the structure of the planet tree, starting with COM
    // int indentLevel = 0;
    // printAllPlanets(com, &indentLevel);

    //meh, got lazy. the tree structure should be there even though we have nodes stored in an array. what i mean is
    //we have mapped all children to their parent. therefore, if we find "COM", then we should be able to traverse the 
    //entire tree and solve the problem
    int nodeCounter = 0;
    int orbitSum = 0;
    countAllOrbits(com, &nodeCounter, &orbitSum);
    printf("numNodes=%i, numOrbits=%i\n", nodeCounter, orbitSum);
    return orbitSum;
}

struct Planet* findLeastCommonAncestor(struct Planet* a, struct Planet* b) {
    //iterate over a
    struct Planet* currentAAncestor = a->parent;
    while (currentAAncestor != NULL) {
        //iterate over b
        struct Planet* currentBAncestor = b->parent;
        while (currentBAncestor != NULL) {
            if (currentBAncestor == currentAAncestor) {
                printf("found least common ancestor: %s\n", currentAAncestor->name);
                return currentAAncestor;
            }
            currentBAncestor = currentBAncestor->parent;
        }
        currentAAncestor = currentAAncestor->parent;
    }
    return NULL;
}

static int countHopsFromChildToParent(struct Planet* child, struct Planet* parent) {
    int count = 0;
    struct Planet* ptr = child;
    while (ptr != parent) {
        printf("hopping from %s to %s\n", ptr->name, ptr->parent->name);
        count++;
        ptr = ptr->parent;
    }
    return count;
}

int countRequiredTransfers(const char* startName, const char* endName, const char** mappings, int numMappings) {
    struct Planet** planets = mapPlanets(mappings, numMappings);
    struct Planet* start = searchForPlanet(planets, numMappings, startName);
    struct Planet* end = searchForPlanet(planets, numMappings, endName);
    struct Planet* leastCommonAncestor = findLeastCommonAncestor(start, end);
    int hopsFromStartToAncestor = countHopsFromChildToParent(start, leastCommonAncestor);
    int hopsFromEndToAncestor = countHopsFromChildToParent(end, leastCommonAncestor);
    int sum = hopsFromStartToAncestor + hopsFromEndToAncestor - 2;  //-2 becuase we're counting start and end as planets. what we really care about is their orbit, which decrements one from each side
    printf("numHops = %i\n", sum);
    return sum;
}