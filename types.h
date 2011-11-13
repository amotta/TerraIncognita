#ifndef TerraIncognita_types_h
#define TerraIncognita_types_h

#include <stdbool.h>

#define FIELD_EMPTY ' '
#define FIELD_ACCESS 'A'
#define FIELD_OBSTACLE 'O'

typedef struct {
    unsigned int rows;
    unsigned int cols;
    unsigned int length;
    char* map;
} map_t;

typedef struct {
    unsigned int top;
    unsigned int bottom;
    unsigned int left;
    unsigned int right;
} obst_t;

typedef struct {
    bool dialogMode;
    bool displayMode;
    map_t map;
    unsigned int numbObsts;
    unsigned int accessRow;
    unsigned int accessCol;
    
    // TODO
    // create robSet_t
    unsigned int numbRobs;
} terra_t;

#endif