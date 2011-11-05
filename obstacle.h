#ifndef TerraIncognita_obstacle_h
#define TerraIncognita_obstacle_h

#include <stdbool.h>

#include "point.h"
#include "map.h"

typedef struct obst {
    point_t topLeft;
    point_t bottomRight;
} obst_t;

typedef struct obstSet {
    unsigned int length;
    obst_t* set;
} obstSet_t;

void obstPrint(obst_t*);
bool obstInMap(obst_t*, map_t*);
void obstSetInit(obstSet_t*);
void obstSetFree(obstSet_t*);

#endif
