#include <stdlib.h>

#include "map.h"
#include "point.h"
#include "robot.h"


void robInit(rob_t* rob){
    rob->active = false;
    rob->explored = false;
}

void robSpawn(terra_t* env){
    rob_t* rob = &env->robs.set[env->robs.spawned];
    
    rob->active = true;
    rob->row = env->accessRow;
    rob->col = env->accessCol;
    rob->mode = MODE_PREPARE;
    
    mapSet(&env->robs.map, rob->row, rob->col, FIELD_ROBOT);
    mapExplore(&env->robs.map, &env->map, rob->row, rob->col);
    
    env->robs.active++;
    env->robs.spawned++;
}

void robEvac(rob_t* rob, terra_t* env){
    rob->active = false;
    env->robs.active--;
    
    mapSet(&env->robs.map, env->accessRow, env->accessCol, FIELD_ACCESS);
}

char robThinkPrepare(rob_t* rob, terra_t* env){
    switch(rob->globalDir){
        case DIR_TOP:
        case DIR_BOTTOM:
            if(rob->col > env->plan.start && robCanMove(rob, DIR_LEFT, env)){
                return DIR_LEFT;
            }
            
            if(rob->col < env->plan.start && robCanMove(rob, DIR_RIGHT, env)){
                return DIR_RIGHT;
            }
            
            if(rob->col == env->plan.start){
                return robThinkExplore(rob, env);
            }
            
            break;
            
        case DIR_LEFT:
        case DIR_RIGHT:
            if(rob->row > env->plan.start && robCanMove(rob, DIR_TOP, env)){
                return DIR_TOP;
            }
            
            if(rob->row < env->plan.start && robCanMove(rob, DIR_BOTTOM, env)){
                return DIR_BOTTOM;
            }
            
            if(rob->row == env->plan.start){
                return robThinkExplore(rob, env);
            }
            
            break;
    }
    
    // in case we end up here:
    // we're not on correct row but
    // we can't move either
    //
    // What we do?
    return robThinkExplore(rob, env);
}

char robThinkExplore(rob_t* rob, terra_t* env){
    rob->mode = MODE_EXPLORE;
    
    // go to next row / col when done
    bool next = false;
    
    switch(rob->dir){
        case DIR_TOP:
            if(rob->row == 0){
                next = true;
                rob->dir = DIR_BOTTOM;
            }
            
            break;
            
        case DIR_BOTTOM:
            if(rob->row == env->map.rows - 1){
                next = true;
                rob->dir = DIR_TOP;
            }
            
            break;
            
        case DIR_LEFT:
            if(rob->col == 0){
                next = true;
                rob->dir = DIR_RIGHT;
            }
            
            break;
            
        case DIR_RIGHT:
            if(rob->col == env->map.cols - 1){
                next = true;
                rob->dir = DIR_LEFT;
            }
            
            break;
    }
    
    if(next){
        switch(rob->globalDir){
            case DIR_TOP:
                if(rob->row >= env->plan.dist){
                    rob->dist -= env->plan.dist;
                }else if(!rob->explored){
                    rob->explored = true;
                    rob->globalDir = DIR_BOTTOM;
                }else{
                    rob->dist = 0;
                }
                break;
                
            case DIR_BOTTOM:
                if(env->map.rows - rob->row > env->plan.dist){
                    rob->dist += env->plan.dist;
                }else if(!rob->explored){
                    rob->explored = true;
                    rob->globalDir = DIR_TOP;
                }else{
                    rob->dist = env->map.rows - 1;
                }
                break;
                
            case DIR_LEFT:
                if(rob->col >= env->plan.dist){
                    rob->dist -= env->plan.dist;
                }else if(!rob->explored){
                    rob->explored = true;
                    rob->globalDir = DIR_RIGHT;
                }else{
                    rob->dist = 0;
                }
                break;
                
            case DIR_RIGHT:
                if(env->map.cols - rob->col > env->plan.dist){
                    rob->dist += env->plan.dist;
                }else if(!rob->explored){
                    rob->explored = true;
                    rob->globalDir = DIR_LEFT;
                }else{
                    rob->dist = env->map.cols - 1;
                }
                break;
        }
    }
    
    switch(rob->globalDir){
        case DIR_TOP:
        case DIR_BOTTOM:
            
            // correct row if needed and possible
            if(rob->row > rob->dist && robCanMove(rob, DIR_TOP, env)){
                return DIR_TOP;
            }
            
            if(rob->row < rob->dist && robCanMove(rob, DIR_BOTTOM, env)){
                return DIR_BOTTOM;
            }
            
            // assume line correct
            switch(rob->dir) {
                case DIR_LEFT:
                    if(robCanMove(rob, DIR_LEFT, env)){
                        return DIR_LEFT;
                    }
                    break;
                    
                case DIR_RIGHT:
                    if(robCanMove(rob, DIR_RIGHT, env)){
                        return DIR_RIGHT;
                    }
                    break;
            }
            
            break;
            
        case DIR_LEFT:
        case DIR_RIGHT:
            if(rob->col > rob->dist && robCanMove(rob, DIR_LEFT, env)){
                return DIR_LEFT;
            }
            
            if(rob->col < rob->dist && robCanMove(rob, DIR_RIGHT, env)){
                return DIR_RIGHT;
            }
            
            switch(rob->dir){
                case DIR_TOP:
                    if(robCanMove(rob, DIR_TOP, env)){
                        return DIR_TOP;
                    }
                    break;
                    
                case DIR_BOTTOM:
                    if(robCanMove(rob, DIR_BOTTOM, env)){
                        return DIR_BOTTOM;
                    }
                    break;
            }

            break;
    }

    return robThinkAvoid(rob, env);
}

char robThinkAvoid(rob_t* rob, terra_t* env){
    rob->mode = MODE_AVOID;
    
    switch(rob->dir){
        case DIR_TOP:
            if(robCanMove(rob, DIR_TOP, env)){
                rob->mode = MODE_EXPLORE;
                return DIR_TOP;
            }
            break;
            
        case DIR_BOTTOM:
            if(robCanMove(rob, DIR_BOTTOM, env)){
                rob->mode = MODE_EXPLORE;
                return DIR_BOTTOM;
            }
            break;
        
        case DIR_LEFT:
            if(robCanMove(rob, DIR_LEFT, env)){
                rob->mode = MODE_EXPLORE;
                return DIR_LEFT;
            }
            break;
        
        case DIR_RIGHT:
            if(robCanMove(rob, DIR_RIGHT, env)){
                rob->mode = MODE_EXPLORE;
                return DIR_RIGHT;
            }
            break;
    }

    switch(rob->globalDir) {
        case DIR_TOP:
            if(robCanMove(rob, DIR_BOTTOM, env)){
                return DIR_BOTTOM;
            }
            break;
            
        case DIR_BOTTOM:
            if(robCanMove(rob, DIR_TOP, env)){
                return DIR_TOP;
            }
            break;
            
        case DIR_LEFT:
            if(robCanMove(rob, DIR_RIGHT, env)){
                return DIR_RIGHT;
            }
            break;
            
        case DIR_RIGHT:
            if(robCanMove(rob, DIR_LEFT, env)){
                return DIR_LEFT;
            }
            break;
        }
    
    return DIR_NONE;
}

char robThink(rob_t* rob, terra_t* env){
    if(
       rob->explored
       && rob->row == env->accessRow
       && rob->col == env->accessCol
    ){
        robEvac(rob, env);
        return DIR_NONE;
    }
    
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
            
        default:
            return;
    }
    
    // explore area
    mapExplore(&env->robs.map, &env->map, newRow, newCol);
    
    // map old robot position
    mapSet(
        &env->robs.map, rob->row, rob->col,
        mapGet(&env->map, rob->row, rob->col)
    );
    
    // map new robot position
    mapSet(&env->robs.map, newRow, newCol, FIELD_ROBOT);
        
    rob->row = newRow;
    rob->col = newCol;
}

bool robCanMove(rob_t* rob, char dir, terra_t* env){
    switch(dir){
        case DIR_TOP:
            if(
               rob->row > 0
               && mapIsEmpty(&env->robs.map, rob->row - 1, rob->col)
            ){
                return true;
            }
            break;
            
        case DIR_BOTTOM:
            if(
               rob->row < env->map.rows - 1
               && mapIsEmpty(&env->robs.map, rob->row + 1, rob->col)
            ){
                return true;
            }
            break;
            
        case DIR_LEFT:
            if(
               rob->col > 0
               && mapIsEmpty(&env->robs.map, rob->row, rob->col - 1)
            ){
                return true;
            }
            break;
            
        case DIR_RIGHT:
            if(
               rob->col < env->map.cols - 1
               && mapIsEmpty(&env->robs.map, rob->row, rob->col + 1)
            ){
                return true;
            }
            break;
    }
               
    return false;
}

void robSetInit(robSet_t* set){
    set->active = 0;
    set->spawned = 0;
    set->planned = 0;
    set->set = (rob_t*) malloc(set->length * sizeof(rob_t));

    unsigned int r;
    for(r = 0; r < set->length; r++){
        robInit(&set->set[r]);
    }
}

void robSetFree(robSet_t* set){
    free(set->set);
    set->set = NULL;
}