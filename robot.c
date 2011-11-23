#include <stdlib.h>

#include "robot.h"

void robInit(rob_t* rob){
    rob->active = false;
}

void robSetInit(robSet_t* set){
    set->active = 0;
    set->planned = 0;
    set->set = (rob_t*) malloc(set->length * sizeof(rob_t));

    unsigned int r;
    for(r = 0; r < set->length; r++){
        robInit(&set->set[r]);
    }
}

void robSetPlan(robSet_t* set, map_t* map){
    // TODO
    // Calculate number of robots
    // and store in set->planned
}

void robSetFree(robSet_t* set){
    free(set->set);
}