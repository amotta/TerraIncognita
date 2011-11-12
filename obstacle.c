#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "point.h"
#include "map.h"
#include "obstacle.h"

/*
void obstAssign(obst_t* dest, obst_t* source){
    dest->top = source->top;
    dest->bottom = source->bottom;
    dest->left = source->left;
    dest->right = source->right;
}
*/

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