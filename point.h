#ifndef TerraIncognita_point_h
#define TerraIncognita_point_h

typedef struct point{
    unsigned int row;
    unsigned int col;
} point_t;

#include <stdbool.h>
#include "map.h"

bool pointInMap(point_t*, map_t*);
bool pointOnBorder(point_t*, map_t*);

#endif