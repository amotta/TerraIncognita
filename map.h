#ifndef TerraIncognita_map_h
#define TerraIncognita_map_h

#define FIELD_EMPTY ' '
#define FIELD_ACCESS 'A'
#define FIELD_OBSTACLE 'O'

typedef struct map {
    unsigned int rows;
    unsigned int cols;
    unsigned int length;
    char* map;
} map_t;

#include "point.h"
#include "obstacle.h"

void mapInit(map_t*, char);
void mapFree(map_t*);
char mapGet(map_t*, unsigned int, unsigned int);
void mapSet(map_t*, unsigned int, unsigned int, char);
void mapAddObstacle(map_t*, obst_t*);
void mapPrint(map_t*);

#endif