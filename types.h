#ifndef TerraIncognita_types_h
#define TerraIncognita_types_h

#include <stdbool.h>

#define FIELD_EMPTY ' '
#define FIELD_ACCESS 'A'
#define FIELD_OBSTACLE 'O'
#define FIELD_ROBOT 'R'
#define FIELD_UNKNOWN '?'

#define DIR_TOP (1 << 0)
#define DIR_BOTTOM (1 << 1)
#define DIR_LEFT (1 << 2)
#define DIR_RIGHT (1 << 3)
#define DIR_UNKNOWN 0

#define MODE_PREPARE 1
#define MODE_EXPLORE 2
#define MODE_AVOID 3

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
    bool active;
    unsigned int dist;
    unsigned int row;
    unsigned int col;
    char mode;
} rob_t;

typedef struct {
    map_t map;
    unsigned int active;
    unsigned int planned;
    unsigned int length;
    rob_t* set;
} robSet_t;

typedef struct {
    unsigned int numbRobs;
    unsigned int dist;
    char dir;
} plan_t;

typedef struct {
    map_t map;
    plan_t plan;
    robSet_t robs;
    obstSet_t obsts;
    bool dialogMode;
    bool resultMode;
    unsigned int accessRow;
    unsigned int accessCol;
} terra_t;

#endif