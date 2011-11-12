#ifndef TerraIncognita_point_h
#define TerraIncognita_point_h

#include <stdbool.h>

#include "types.h"

bool pointInMap(unsigned int, unsigned int, map_t*);
bool pointOnBorder(unsigned int, unsigned int, map_t*);

#endif