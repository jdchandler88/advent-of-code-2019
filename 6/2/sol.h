typedef struct Planet {
  const char* name;
  struct Planet* parent;
  struct Planet** children;
  int numChildren;
} Planet;

int countOrbits(const char** mappings, int numMappings);

int countRequiredTransfers(const char* start, const char* end, const char** mappings, int numMappings);

struct Planet* findLeastCommonAncestor(struct Planet* a, struct Planet* b);