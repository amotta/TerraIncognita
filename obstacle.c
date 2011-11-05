#include <stdlib.h>

#include "obstacle.h"

void obstSetInit(obstSet_t* set){
    set->set = (obst_t*) malloc(set->length * sizeof(obst_t));
}

void obstSetFree(obstSet_t* set){
    free(set->set);
}