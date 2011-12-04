#ifndef TerraIncognita_map_h
#define TerraIncognita_map_h

#include "types.h"

void mapInit(map_t*, char);
void mapFree(map_t*);
char mapGet(map_t*, unsigned int, unsigned int);
void mapSet(map_t*, unsigned int, unsigned int, char);
bool mapIsComplete(map_t* map);
bool mapIsEmpty(map_t* map, unsigned int row, unsigned int col);
bool mapIsObstacle(map_t* map, unsigned int row, unsigned int col);
void mapExplore(map_t* dest, map_t* src, unsigned int r, unsigned int c);
void mapComplete(map_t* dest, map_t* src);
void mapCompleteField(map_t* dest, map_t* src, unsigned int r, unsigned int c);
void mapExploreBorder(map_t* dest, map_t* src);
void mapAddObstacle(map_t*, obst_t*);
void mapPrint(map_t*);

#endif