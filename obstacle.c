#include <stdlib.h>
#include <stdio.h>

#include "obstacle.h"

void obstPrint(obst_t* obst){
    printf("%u|%u\t", obst->topLeft.row, obst->topLeft.col);
    printf("%u|%u\n", obst->topLeft.row, obst->bottomRight.col);
    printf("%u|%u\t", obst->bottomRight.row, obst->topLeft.col);
    printf("%u|%u\n", obst->bottomRight.row, obst->bottomRight.col);
}

void obstSetInit(obstSet_t* set){
    set->set = (obst_t*) malloc(set->length * sizeof(obst_t));
}

void obstSetFree(obstSet_t* set){
    free(set->set);
}