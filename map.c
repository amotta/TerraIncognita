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

void mapAddObstacle(map_t* map, obst_t* obst){
    unsigned int r;
    unsigned int c;
    
    for(r = obst->top; r <= obst->bottom; r++){
        for(c = obst->left; c <= obst->right; c++){
            mapSet(map, r, c, FIELD_OBSTACLE);
        }
    }
}

void mapPrint(map_t* map){
    unsigned int i, j;
    for(i = 0; i < map->rows; i++){
        for(j = 0; j < 2 * map->cols + 1; j++){
            putchar('-');
        }
        
        putchar('\n');
        
        for(j = 0; j < map->cols; j++){
            putchar('|');
            putchar(mapGet(map, i, j));
        }
        
        putchar('|');
        putchar('\n');
    }
    
    for(j = 0; j < 2 * map->cols; j++){
        putchar('-');
    }
    
    putchar('\n');
}