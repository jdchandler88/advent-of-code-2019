typedef struct Planet {
  const char* name;
  struct Planet* parent;
  struct Planet** children;
  int numChildren;
} Planet;

int countOrbits(const char** mappings, int numMappings);