#ifndef TerraIncognita_obstacle_h
#define TerraIncognita_obstacle_h

#include <stdbool.h>

#include "types.h"

void obstPrint(obst_t*);
bool obstInMap(obst_t*, map_t*);
bool obstOnBorder(obst_t*, map_t*);
bool obstCollides(obst_t*, map_t*);

#endif