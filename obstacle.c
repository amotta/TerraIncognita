#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "point.h"
#include "map.h"
#include "obstacle.h"

void obstPrint(obst_t* obst){
    printf("%u-%u ", obst->top, obst->bottom);
    printf("%u-%u\n", obst->left, obst->right);
}

bool obstInMap(obst_t* obst, map_t* map){
    if(
       pointInMap(obst->top, obst->left, map)
       && pointInMap(obst->bottom, obst->right, map)
    ){
        return true;
    }else{
        return false;
    }
}

bool obstOnBorder(obst_t* obst, map_t* map){
    if(
       pointOnBorder(obst->top, obst->left, map)
       || pointOnBorder(obst->bottom, obst->right, map)
    ){
        return true;
    }else{
        return false;
    }
}

bool obstCollides(obst_t* obst, map_t* map){
    unsigned int r;
    unsigned int c;
    
    for(r = obst->top - 1; r <= obst->bottom + 1; r++){
        for(c = obst->left - 1; c <= obst->right + 1; c++){
            if(mapGet(map, r, c) == FIELD_OBSTACLE){
                return true;
            }
        }
    }
    
    return false;
}