#ifndef TerraIncognita_types_h
#define TerraIncognita_types_h

#define FIELD_EMPTY ' '
#define FIELD_ACCESS 'A'
#define FIELD_OBSTACLE 'O'

#define MODE_CONSOLE 0
#define MODE_REDIRECT 1

typedef struct map {
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

#endif