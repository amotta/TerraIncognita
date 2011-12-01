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
            if(
               rob->col > env->plan.start
               && mapIsEmpty(&env->robs.map, rob->row, rob->col - 1)
            ){
                return DIR_LEFT;
            }
            
            if(
               rob->col < env->plan.start
               && mapIsEmpty(&env->robs.map, rob->row, rob->col + 1)
            ){
                return DIR_RIGHT;
            }
            
            if(rob->col == env->plan.start){
                rob->mode = MODE_EXPLORE;
                rob->modeChanged = true;
            }
            
            break;
            
        case DIR_LEFT:
        case DIR_RIGHT:
            if(
               rob->row > env->plan.start
               && mapIsEmpty(&env->robs.map, rob->row - 1, rob->col)
            ){
                return DIR_TOP;
            }
            
            if(
               rob->row < env->plan.start
               && mapIsEmpty(&env->robs.map, rob->row + 1, rob->col)
            ){
                return DIR_BOTTOM;
            }
            
            if(rob->row == env->plan.start){
                rob->mode = MODE_EXPLORE;
                rob->modeChanged = true;
            }
            
            break;
    }
    
    return DIR_NONE;
}

char robThinkExplore(rob_t* rob, terra_t* env){
    // TODO
    // add final condition
    
    // TODO
    // Move in other switch
    
    // go to next row / col when done with current
    switch(rob->dir){
        case DIR_TOP:
            if(rob->row == 0){
                rob->dir = DIR_BOTTOM;
                rob->dist += env->plan.dist;
            }
            
            break;
            
        case DIR_BOTTOM:
            if(rob->row == env->map.rows - 1){
                rob->dir = DIR_TOP;
                rob->dist += env->plan.dist;
            }
            
            break;
            
        case DIR_LEFT:
            if(rob->col == 0){
                rob->dir = DIR_RIGHT;
                rob->dist += env->plan.dist;
            }
            
            break;
            
        case DIR_RIGHT:
            if(rob->col == env->map.cols - 1){
                rob->dir = DIR_LEFT;
                rob->dist += env->plan.dist;
            }
            
            break;
    }
    
    switch(env->plan.dir){
        case DIR_TOP:
        case DIR_BOTTOM:
            // correct row if needed and possible
            if(
               rob->row > rob->dist
               && mapIsEmpty(&env->robs.map, rob->row - 1, rob->col)
            ){
                return DIR_TOP;
            }
            
            if(
               rob->row < rob->dist
               && mapIsEmpty(&env->robs.map, rob->row + 1, rob->col)
            ){
                return DIR_BOTTOM;
            }
            
            // assume line correct
            switch(rob->dir) {
                case DIR_LEFT:
                    if(mapIsEmpty(&env->robs.map, rob->row, rob->col - 1)){
                        return DIR_LEFT;
                    }
                    break;
                    
                case DIR_RIGHT:
                    if(mapIsEmpty(&env->robs.map, rob->row, rob->col + 1)){
                        return DIR_RIGHT;
                    }
                    break;
            }
            
            break;
            
        case DIR_LEFT:
        case DIR_RIGHT:
            if(
               rob->col > rob->dist
               && mapIsEmpty(&env->robs.map, rob->row, rob->col - 1)
            ){
                return DIR_LEFT;
            }
            
            if(rob->col < rob->dist
               && mapIsEmpty(&env->robs.map, rob->row, rob->col + 1)
            ){
                return DIR_RIGHT;
            }
            
            switch(rob->dir){
                case DIR_TOP:
                    if(mapIsEmpty(&env->robs.map, rob->row - 1, rob->col)){
                        return DIR_TOP;
                    }
                    break;
                    
                case DIR_BOTTOM:
                    if(mapIsEmpty(&env->robs.map, rob->row + 1, rob->col)){
                        return DIR_BOTTOM;
                    }
                    break;
            }

            break;
    }
    
    // TODO
    rob->mode = MODE_AVOID;
    rob->modeChanged = true;
    
    return DIR_NONE;
}

char robThinkAvoid(rob_t* rob, terra_t* env){
    switch(rob->dir){
        case DIR_TOP:
            if(mapIsEmpty(&env->robs.map, rob->row - 1, rob->col)){
                rob->mode = MODE_EXPLORE;
                return DIR_TOP;
            }
            break;
        
        case DIR_BOTTOM:
            if(mapIsEmpty(&env->robs.map, rob->row + 1, rob->col)){
                rob->mode = MODE_EXPLORE;
                return DIR_BOTTOM;
            }
            break;
        
        case DIR_LEFT:
            if(mapIsEmpty(&env->robs.map, rob->row, rob->col - 1)){
                rob->mode = MODE_EXPLORE;
                return DIR_LEFT;
            }
            break;
        
        case DIR_RIGHT:
            if(mapIsEmpty(&env->robs.map, rob->row, rob->col + 1)){
                rob->mode = MODE_EXPLORE;
                return DIR_RIGHT;
            }
            break;
    }

    // TODO
    // Check conditions
    switch(env->plan.dir) {
        case DIR_TOP:
            return DIR_BOTTOM;
            
        case DIR_BOTTOM:
            return DIR_TOP;
            
        case DIR_LEFT:
            return DIR_RIGHT;
            
        case DIR_RIGHT:
            return DIR_LEFT;
    }
    
    return DIR_NONE;
}

char robThink(rob_t* rob, terra_t* env){
    char dir;
    
    rob->modeChanged = false;
    
    switch(rob->mode){
        case MODE_PREPARE:
            dir = robThinkPrepare(rob, env);
            break;
            
        case MODE_EXPLORE:
            dir = robThinkExplore(rob, env);
            break;
            
        case MODE_AVOID:
            dir = robThinkAvoid(rob, env);
            break;
    }
    
    if(rob->modeChanged){
        dir = robThink(rob, env);
    }
    
    return dir;
}

void robMove(rob_t* rob, char dir, terra_t* env){
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
    
    // only move on empty fields
    if(mapIsEmpty(&env->robs.map, newRow, newCol)){
        // explore area
        mapExplore(&env->robs.map, &env->map, newRow, newCol);
        
        // map old robot position
        mapSet(
            &env->robs.map,
            rob->row,
            rob->col,
            mapGet(&env->map, rob->row, rob->col)
        );
        
        // map new robot position
        mapSet(&env->robs.map, newRow, newCol, FIELD_ROBOT);
        
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