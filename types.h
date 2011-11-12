#ifndef TerraIncognita_types_h
#define TerraIncognita_types_h

#define FIELD_EMPTY ' '
#define FIELD_ACCESS 'A'
#define FIELD_OBSTACLE 'O'

#define MODE_CONSOLE 0
#define MODE_REDIRECT 1

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
    char dialogMode;
    char displayMode;
    map_t map;
    unsigned int numbObsts;
    unsigned int accessRow;
    unsigned int accessCol;
    
    // TODO
    // create robSet_t
    unsigned int numbRobs;
} terra_t;

#endif