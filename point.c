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