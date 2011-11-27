#include <stdlib.h>

#include "point.h"
#include "robot.h"


void robInit(rob_t* rob){
    rob->active = false;
}

void robSpawn(terra_t* env){
    rob_t* rob = &env->robs.set[env->robs.active];
    
    rob->active = true;
    rob->row = env->accessRow;
    rob->col = env->accessCol;
    
    env->robs.active++;
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

void robSetFree(robSet_t* set){
    free(set->set);
}