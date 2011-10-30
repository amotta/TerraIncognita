#ifndef TerraIncognita_map_h
#define TerraIncognita_map_h

#include "point.h"

#define FIELD_EMPTY ' '
#define FIELD_ACCESS 'A'

typedef struct map {
    unsigned int rows;
    unsigned int cols;
    unsigned int length;
    char* map;
} map_t;

void mapInit(map_t*, char);
void mapFree(map_t*);
char mapGet(map_t*, unsigned int, unsigned int);
void mapSet(map_t*, unsigned int, unsigned int, char);
void mapSetAccessPoint(map_t*, point_t*);
void mapPrint(map_t*);

#endif