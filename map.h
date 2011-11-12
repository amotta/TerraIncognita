#ifndef TerraIncognita_map_h
#define TerraIncognita_map_h

#include "types.h"

void mapInit(map_t*, char);
void mapFree(map_t*);
char mapGet(map_t*, unsigned int, unsigned int);
void mapSet(map_t*, unsigned int, unsigned int, char);
void mapAddObstacle(map_t*, obst_t*);
void mapPrint(map_t*);

#endif