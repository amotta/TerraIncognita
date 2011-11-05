#include <stdlib.h>

#include "obstacle.h"

void obstSetInit(obstSet_t* set){
    set->set = (obst_t*) malloc(set->length * sizeof(obst_t));
}