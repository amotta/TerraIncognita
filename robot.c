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
    rob->mode = MODE_PREPARE;
    
    mapExplore(&env->robs.map, &env->map, rob->row, rob->col);
}

void robMovePrepare(rob_t* rob, terra_t* env){
    switch(env->plan.dir){
        case DIR_TOP:
        case DIR_BOTTOM:
            if(rob->col == env->plan.start){
                if(rob->row > rob->dist){
                    rob->row--;
                }
                
                if(rob->row < rob->dist){
                    rob->row++;
                }
                
                if(rob->row == rob->dist){
                    rob->mode = MODE_EXPLORE;
            
                }
            }
            
            if(rob->col > env->plan.start){
                rob->col--;
            }
            
            if(rob->col < env->plan.start){
                rob->col++;
            }
            break;
            
        case DIR_LEFT:
        case DIR_RIGHT:
            if(rob->row == env->plan.start){
                if(rob->col > rob->dist){
                    rob->col--;
                }
                
                if(rob->col < rob->dist){
                    rob->col++;
                }
                
                if(rob->col == rob->dist){
                    rob->mode = MODE_EXPLORE;
                }
            }
            
            if(rob->row > env->plan.start){
                rob->row--;
            }
            
            if(rob->row < env->plan.start){
                rob->row++;
            }
            break;
    }
}

void robMoveExplore(rob_t* rob, terra_t* env){
    switch(rob->dir){
        case DIR_TOP:
            rob->row--;
            break;
        case DIR_BOTTOM:
            rob->row++;
            break;
        case DIR_LEFT:
            rob->col--;
            break;
        case DIR_RIGHT:
            rob->col++;
            break;
    }
}

void robMoveAvoid(rob_t* rob, terra_t* env){
    
}

void robMove(rob_t* rob, terra_t* env){
    switch(rob->mode){
        case MODE_PREPARE:
            robMovePrepare(rob, env);
            break;
        case MODE_EXPLORE:
            robMoveExplore(rob, env);
            break;
        case MODE_AVOID:
            robMoveAvoid(rob, env);
            break;
    }
    
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