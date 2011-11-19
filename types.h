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
    unsigned int used;
    unsigned int length;
    obst_t* set;
} obstSet_t;

typedef struct {
    bool dialogMode;
    bool resultMode;
    map_t map;
    obstSet_t obsts;
    unsigned int accessRow;
    unsigned int accessCol;
    
    // TODO
    // create robSet_t
    unsigned int numbRobs;
} terra_t;

#define MODE_NONE 0x0
#define MODE_ACTIVE 0x1
#define MODE_EXPLORE 0x2
#define MODE_OBSTACLE = 0x4

typedef struct {
    char mode;
    char move;
    unsigned int row;
    unsigned int col;
} rob_t;

typedef struct {
    unsigned int active;
    unsigned int length;
    rob_t* set;
} robSet_t;

#endif