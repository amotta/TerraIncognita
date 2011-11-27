#include <stdlib.h>

#include "map.h"
#include "point.h"
#include "robot.h"


void robInit(rob_t* rob){
    rob->active = false;
}

void robSpawn(rob_t* rob, terra_t* env){
    rob->active = true;
    rob->row = env->accessRow;
    rob->col = env->accessCol;
    rob->mode = MODE_EXPLORE;
    
    mapExplore(&env->robs.map, &env->map, rob->row, rob->col);
}

void robMove(rob_t* rob, terra_t* env){
    rob->row++;
    
    mapExplore(&env->robs.map, &env->map, rob->row, rob->col);
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