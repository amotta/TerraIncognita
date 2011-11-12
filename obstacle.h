#ifndef TerraIncognita_obstacle_h
#define TerraIncognita_obstacle_h

#include <stdbool.h>

#include "map.h"

typedef struct {
    unsigned int top;
    unsigned int bottom;
    unsigned int left;
    unsigned int right;
} obst_t;

void obstPrint(obst_t*);
bool obstInMap(obst_t*, map_t*);
bool obstOnBorder(obst_t*, map_t*);

#endif