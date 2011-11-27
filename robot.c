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
    
    mapSet(&env->robs.map, rob->row, rob->col, FIELD_ROBOT);
    mapExplore(&env->robs.map, &env->map, rob->row, rob->col);
}

char robThinkPrepare(rob_t* rob, terra_t* env){
    switch(env->plan.dir){
        case DIR_TOP:
        case DIR_BOTTOM:
            if(rob->col == env->plan.start){
                if(rob->row > rob->dist){
                    return DIR_TOP;
                }
                
                if(rob->row < rob->dist){
                    return DIR_BOTTOM;
                }
                
                if(rob->row == rob->dist){
                    rob->mode = MODE_EXPLORE;
                }
            }
            
            if(rob->col > env->plan.start){
                return DIR_LEFT;
            }
            
            if(rob->col < env->plan.start){
                return DIR_RIGHT;
            }
            
            break;
            
        case DIR_LEFT:
        case DIR_RIGHT:
            if(rob->row == env->plan.start){
                if(rob->col > rob->dist){
                    return DIR_LEFT;
                }
                
                if(rob->col < rob->dist){
                    return DIR_RIGHT;
                }
                
                if(rob->col == rob->dist){
                    rob->mode = MODE_EXPLORE;
                }
            }
            
            if(rob->row > env->plan.start){
                return DIR_TOP;
            }
            
            if(rob->row < env->plan.start){
                return DIR_BOTTOM;
            }
            
            break;
    }
    
    return DIR_NONE;
}

char robThinkExplore(rob_t* rob, terra_t* env){
    switch(rob->dir){
        case DIR_TOP:
            return DIR_TOP;
        case DIR_BOTTOM:
            return DIR_BOTTOM;
        case DIR_LEFT:
            return DIR_LEFT;
        case DIR_RIGHT:
            return DIR_RIGHT;
    }
    
    return DIR_NONE;
}

char robThinkAvoid(rob_t* rob, terra_t* env){
    return DIR_NONE;
}

char robThink(rob_t* rob, terra_t* env){
    switch(rob->mode){
        case MODE_PREPARE:
            return robThinkPrepare(rob, env);
        case MODE_EXPLORE:
            return robThinkExplore(rob, env);
        case MODE_AVOID:
            return robThinkAvoid(rob, env);
    }
    
    return DIR_NONE;
}

void robMove(rob_t* rob, char dir, terra_t* env){
    char field;
    unsigned int newRow = rob->row;
    unsigned int newCol = rob->col;
    
    switch(dir){
        case DIR_TOP:
            newRow--;
            break;
        case DIR_BOTTOM:
            newRow++;
            break;
        case DIR_LEFT:
            newCol--;
            break;
        case DIR_RIGHT:
            newCol++;
            break;
    }
    
    field = mapGet(&env->robs.map, newRow, newCol);
    if(field == FIELD_EMPTY || field == FIELD_ACCESS){
        mapExplore(&env->robs.map, &env->map, newRow, newCol);
        
        mapSet(
            &env->robs.map,
            rob->row,
            rob->col,
            mapGet(&env->map, rob->row, rob->col)
        );
        
        mapSet(
            &env->robs.map,
            newRow,
            newCol,
            FIELD_ROBOT
        );
        
        rob->row = newRow;
        rob->col = newCol;
    }
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