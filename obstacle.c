#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "obstacle.h"
#include "point.h"
#include "map.h"

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

void obstSetInit(obstSet_t* set){
    set->set = (obst_t*) malloc(set->length * sizeof(obst_t));
}

void obstSetFree(obstSet_t* set){
    free(set->set);
}