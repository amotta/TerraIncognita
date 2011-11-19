#include <stdlib.h>

#include "robot.h"

void robInit(rob_t* rob){
    rob->mode = MODE_NONE;
}

void robSetInit(robSet_t* set){
    set->active = 0;
    set->set = (rob_t*) malloc(set->length * sizeof(rob_t));

    unsigned int r;
    for(r = 0; r < set->length; r++){
        robInit(&set->set[r]);
    }
}

void robSetFree(robSet_t* set){
    free(set->set);
}