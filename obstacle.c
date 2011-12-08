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

void obstSetInit(obstSet_t* set){
    set->used = 0;
    set->set = (obst_t*) malloc(set->length * sizeof(obst_t));
}

int obstSetCheck(obstSet_t* set, obst_t* obst){
    unsigned int o;
    
    for(o = 0; o < set->used; o++){
        if(
           obst->top - 1 <= set->set[o].top
           && obst->bottom + 1 >= set->set[o].bottom
           && obst->left - 1 <= set->set[o].left
           && obst->right + 1 >= set->set[o].right
        ){
            return o;
        }
    }
    
    return -1;
}

void obstSetAdd(obstSet_t* set, obst_t* obst){
    if(set->used < set->length){
        set->set[set->used].top = obst->top;
        set->set[set->used].bottom = obst->bottom;
        set->set[set->used].left = obst->left;
        set->set[set->used].right = obst->right;
        set->used++;
    }
}

void obstSetFree(obstSet_t* set){
    free(set->set);
    set->set = NULL;
}