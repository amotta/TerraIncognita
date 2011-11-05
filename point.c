#include "point.h"

bool pointInMap(point_t* point, map_t* map){
    if(point->row < map->rows && point->col < map->cols){
        return true;
    }else{
        return false;
    }
}

bool pointOnBorder(point_t* point, map_t* map){
    if(
       point->row == 0 || point->row == map->rows - 1
       || point->col == 0 || point->col == map->cols - 1
    ){
        return true;
    }else{
        return false;
    }
}