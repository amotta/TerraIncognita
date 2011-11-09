#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "point.h"
#include "map.h"
#include "obstacle.h"

void obstAssign(obst_t* dest, obst_t* source){
    dest->topLeft.row = source->topLeft.row;
    dest->topLeft.col = source->topLeft.col;
    dest->bottomRight.row = source->bottomRight.row;
    dest->bottomRight.col = source->bottomRight.col;
}

void obstPrint(obst_t* obst){
    printf("%u|%u\t", obst->topLeft.row, obst->topLeft.col);
    printf("%u|%u\n", obst->topLeft.row, obst->bottomRight.col);
    printf("%u|%u\t", obst->bottomRight.row, obst->topLeft.col);
    printf("%u|%u\n", obst->bottomRight.row, obst->bottomRight.col);
}

bool obstInMap(obst_t* obst, map_t* map){
    if(pointInMap(&obst->topLeft, map) && pointInMap(&obst->bottomRight, map)){
        return true;
    }else{
        return false;
    }
}

bool obstOnBorder(obst_t* obst, map_t* map){
    if(
       pointOnBorder(&obst->topLeft, map)
       || pointOnBorder(&obst->bottomRight, map)
    ){
        return true;
    }else{
        return false;
    }
}

bool obstIsSeparate(obst_t* obst, obstSet_t* obstSet){
    int i;
    bool separate = true;
    
    for(i = 0; separate && i < obstSet->numbObsts; i++){
        if(
           obst->topLeft.row - 1 <= obstSet->set[i].bottomRight.row
           && obst->bottomRight.row + 1 >= obstSet->set[i].topLeft.row
           && obst->topLeft.col - 1 <= obstSet->set[i].bottomRight.col
           && obst->bottomRight.col + 1 >= obstSet->set[i].topLeft.col
        ){
            separate = false;
        }
    }
    
    return separate;
}

void obstSetPrint(obstSet_t* set){
    int i;
    
    printf("ObstacleSet contains %d obstacles\n", set->numbObsts);
    
    for(i = 0; i < set->numbObsts; i++){
        obstPrint(&(set->set[i]));
    }
}

void obstSetInit(obstSet_t* set){
    set->numbObsts = 0;
    set->set = (obst_t*) malloc(set->length * sizeof(obst_t));
}

void obstSetAdd(obstSet_t* set, obst_t* obst){
    if(set->numbObsts < set->length){
        obstAssign(&(set->set[set->numbObsts]), obst);
        set->numbObsts++;
    }
}

void obstSetFree(obstSet_t* set){
    free(set->set);
}