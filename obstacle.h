#ifndef TerraIncognita_obstacle_h
#define TerraIncognita_obstacle_h

#include "point.h"

typedef struct {
    unsigned int top;
    unsigned int bottom;
    unsigned int left;
    unsigned int right;
} obst_t;

typedef struct obstSet {
    unsigned int length;
    unsigned int used;
    obst_t* set;
} obstSet_t;

#include <stdbool.h>
#include "map.h"

void obstAssign(obst_t*, obst_t*);
void obstPrint(obst_t*);
bool obstInMap(obst_t*, map_t*);
bool obstOnBorder(obst_t*, map_t*);
bool obstIsSeparate(obst_t*, obstSet_t*);
void obstSetPrint(obstSet_t*);
void obstSetInit(obstSet_t*);
void obstSetAdd(obstSet_t*, obst_t*);
void obstSetFree(obstSet_t*);

#endif
