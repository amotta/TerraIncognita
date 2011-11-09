#include <stdlib.h>
#include <stdio.h>

#include "map.h"
#include "point.h"

void mapInit(map_t* map, char fill){
    map->length = map->rows * map->cols;
    map->map = (char*) malloc(map->length * sizeof(char));
    
    unsigned int col, row;
    for(row = 0; row < map->rows; row++){
        for(col = 0; col < map->cols; col++){
            mapSet(map, row, col, fill);
        }
    }
}

void mapFree(map_t* map){
    free(map->map);
}

char mapGet(map_t* map, unsigned int row, unsigned int col){
    unsigned int i = row * map->cols + col;
    
    if(i < map->length){
        return map->map[i];
    }else{
        return 0;
    }
}

void mapSet(map_t* map, unsigned int row, unsigned int col, char set){
    unsigned int i = row * map->cols + col;
    
    if(i < map->length){
        map->map[i] = set;
    }
}

void mapAddObstacles(map_t* map, obstSet_t* obstSet){
    unsigned int i;
    unsigned int r;
    unsigned int c;
    obst_t* obst;
    
    for(i = 0; i < obstSet->numbObsts; i++){
        obst = &obstSet->set[i];
        
        for(r = obst->topLeft.row; r < obst->bottomRight.row; r++){
            for(c = obst->topLeft.col; c < obst->bottomRight.col; c++){
                mapSet(map, r, c, FIELD_OBSTACLE);
            }
        }
    }
}

void mapSetAccessPoint(map_t* map, point_t* point){
    mapSet(map, point->row, point->col, FIELD_ACCESS);
}

void mapPrint(map_t* map){
    unsigned int i, j;
    for(i = 0; i < map->rows; i++){
        for(j = 0; j < map->cols; j++){
            putchar(mapGet(map, i, j));
        }
        
        putchar('\n');
    }
    
    fflush(stdout);
}