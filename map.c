#include <stdlib.h>
#include <stdio.h>

#include "point.h"
#include "map.h"

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
    map->map = NULL;
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

bool mapIsEmpty(map_t* map, unsigned int row, unsigned col){
    char field = mapGet(map, row, col);
    
    if(field == FIELD_EMPTY || field == FIELD_ACCESS){
        return true;
    }else{
        return false;
    }
}

bool mapIsObstacle(map_t* map, unsigned int row, unsigned int col){
    if(mapGet(map, row, col) == FIELD_OBSTACLE){
        return true;
    }else{
        return false;
    }
}

void mapExplore(map_t* dest, map_t* src, unsigned int r, unsigned int c){
    if(pointInMap(r - 1, c, dest) && mapGet(dest, r - 1, c) == FIELD_UNKNOWN){
        mapSet(dest, r - 1, c, mapGet(src, r - 1, c)); 
    }
    
    if(pointInMap(r + 1, c, dest) && mapGet(dest, r + 1, c) == FIELD_UNKNOWN){
        mapSet(dest, r + 1, c, mapGet(src, r + 1, c)); 
    }
    
    if(pointInMap(r, c - 1, dest) && mapGet(dest, r, c - 1) == FIELD_UNKNOWN){
        mapSet(dest, r, c - 1, mapGet(src, r, c - 1));
    }
    
    if(pointInMap(r, c + 1, dest) && mapGet(dest, r, c + 1) == FIELD_UNKNOWN){
        mapSet(dest, r, c + 1, mapGet(src, r, c + 1));
    }
}

void mapExploreBorder(map_t* dest, map_t* src){
    unsigned int i;
    
    for(i = 0; i < dest->rows; i++){
        mapSet(dest, i, 0, mapGet(src, i, 0));
        mapSet(dest, i, dest->cols - 1, mapGet(src, i, dest->cols - 1));
    }
    
    for(i = 0; i < dest->cols; i++){
        mapSet(dest, 0, i, mapGet(src, 0, i));
        mapSet(dest, dest->rows - 1, i, mapGet(src, dest->rows - 1, i));
    }
}

void mapComplete(map_t* dest, map_t* src){
    unsigned int r;
    unsigned int c;
    
    for(r = 1; r < dest->rows - 1; r++){
        for(c = 1; c < dest->cols - 1; c++){
            mapCompleteField(dest, src, r, c);
        }
    }
}

void mapCompleteField(map_t* dest, map_t* src, unsigned int r, unsigned int c){
    if(
        mapGet(dest, r, c) == FIELD_UNKNOWN
        && (mapIsObstacle(dest, r - 1, c) || mapIsObstacle(dest, r + 1, c))
        && (mapIsObstacle(dest, r, c - 1) || mapIsObstacle(dest, r, c + 1))
    ){
        mapSet(dest, r, c, FIELD_OBSTACLE);
        
        mapCompleteField(dest, src, r - 1, c);
        mapCompleteField(dest, src, r + 1, c);
        mapCompleteField(dest, src, r, c - 1);
        mapCompleteField(dest, src, r, c + 1);
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