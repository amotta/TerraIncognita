#include <stdlib.h>

#include "robot.h"

void robSetInit(robSet_t* set){
    set->active = 0;
    set->set = (rob_t*) malloc(set->length * sizeof(rob_t));
}

void robSetFree(robSet_t* set){
    free(set->set);
}