#include <stdio.h>

#include "point.h"

bool pointInMap(unsigned int row, unsigned int col, map_t* map){
    if(row < map->rows && col < map->cols){
        return true;
    }else{
        return false;
    }
}

bool pointOnBorder(unsigned int row, unsigned int col, map_t* map){
    if(row == 0 || col == 0 || row == map->rows - 1 || col == map->cols - 1){
        return true;
    }else{
        return false;
    }
}

char pointGetBorder(unsigned int row, unsigned int col, map_t* map){
    if(row == 0){
        return DIR_TOP;
    }
    
    if(col == 0){
        return DIR_LEFT;
    }
    
    if(row == map->rows - 1){
        return DIR_BOTTOM;
    }
    
    if(col == map->cols -1 ){
        return DIR_RIGHT;
    }
    
    printf("ERROR: Point not on border.\n");
    return DIR_NONE;
}