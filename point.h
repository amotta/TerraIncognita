#ifndef TerraIncognita_point_h
#define TerraIncognita_point_h

#include <stdbool.h>

#include "types.h"

bool pointInMap(unsigned int row, unsigned int col, map_t* map);
bool pointOnBorder(unsigned int row, unsigned int  col, map_t* map);
char pointGetBorder(unsigned int row, unsigned int col, map_t* map);

#endif