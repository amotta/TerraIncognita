// TERRA INCOGNITA
// Rendu final
// 09. 12. 2011
// 
// Alessandro Motta
// 2011-MT1
//
// main.c

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// contents of types.h
#define FIELD_EMPTY ' '
#define FIELD_ACCESS 'A'
#define FIELD_OBSTACLE 'O'
#define FIELD_ROBOT 'R'
#define FIELD_UNKNOWN '?'

#define DIR_TOP (1 << 0)
#define DIR_BOTTOM (1 << 1)
#define DIR_LEFT (1 << 2)
#define DIR_RIGHT (1 << 3)
#define DIR_NONE 0

#define MODE_PREPARE 1
#define MODE_EXPLORE 2
#define MODE_AVOID 3

#define SUCCESS 1
#define CONSTR_TIMEOUT 2
#define EVAC_TIMEOUT 3

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
    bool explored;
    unsigned int dist;
    unsigned int row;
    unsigned int col;
    char dir;
    char globalDir;
    char mode;
} rob_t;

typedef struct {
    map_t map;
    unsigned int active;
    unsigned int spawned;
    unsigned int planned;
    unsigned int length;
    rob_t* set;
} robSet_t;

typedef struct {
    unsigned int dist;
    unsigned int numbRobs;
    unsigned int start;
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
    
    char result;
    unsigned int numbCycles;
} terra_t;


// contents of main.h
void emptyStdIn(void);
void printPrompt(void);
bool readDialogMode(terra_t* env);
bool readResultMode(terra_t* env);
bool readMapSize(terra_t* env);
bool readAccessPoint(terra_t* env);
bool readNumbRobs(terra_t* env);
bool readNumbObsts(terra_t* env);
bool readObsts(terra_t* env);
bool readCoord(unsigned int* ptr);
bool readData(terra_t* env);
void plan(terra_t* env);
bool init(terra_t* env);
void loop(terra_t* env);
void stats(terra_t* env);
void clean(terra_t* env);
int main(void);

// contents of map.h
void mapInit(map_t* map, char fill);
void mapFree(map_t* map);
char mapGet(map_t* map, unsigned int row, unsigned int col);
void mapSet(map_t* map, unsigned int row, unsigned int col, char fill);
bool mapIsEmpty(map_t* map, unsigned int row, unsigned int col);
bool mapIsObstacle(map_t* map, unsigned int row, unsigned int col);
bool mapIsComplete(map_t* map);
void mapExplore(map_t* dest, map_t* src, unsigned int row, unsigned int col);
void mapComplete(map_t* dest, map_t* src);
void mapCompleteField(
    map_t* dest, map_t* src, unsigned int row, unsigned int col
);
void mapExploreBorder(map_t* dest, map_t* src);
void mapAddObstacle(map_t* map, obst_t* obst);
void mapPrint(map_t* map);
unsigned int mapFieldsAnalyzed(map_t* map);
unsigned int mapFieldsNotObst(map_t* map);


// contents of obstacle.h
void obstPrint(obst_t* obst);
bool obstInMap(obst_t* obst, map_t* map);
bool obstOnBorder(obst_t* obst, map_t* map);
void obstSetInit(obstSet_t* set);
int obstSetCheck(obstSet_t* set, obst_t* obst);
void obstSetAdd(obstSet_t* set, obst_t* obst);
void obstSetFree(obstSet_t* set);


// contents of point.h
bool pointInMap(unsigned int row, unsigned int col, map_t* map);
bool pointOnBorder(unsigned int row, unsigned int  col, map_t* map);
char pointGetBorder(unsigned int row, unsigned int col, map_t* map);


// contents of robot.h
void robInit(rob_t* rob);
void robSpawn(terra_t* env);
void robReturn(rob_t* rob, terra_t* env);
void robEvac(rob_t* rob, terra_t* env);
char robThinkPrepare(rob_t* rob, terra_t* env);
char robThinkExplore(rob_t* rob, terra_t* env);
char robThinkAvoid(rob_t* rob, terra_t* env);
char robThink(rob_t* rob, terra_t* env);
void robMove(rob_t* rob, char dir, terra_t* env);
bool robCanMove(rob_t* rob, char dir, terra_t* env);
void robSetInit(robSet_t* set);
void robSetFree(robSet_t* set);

// contents of point.c
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
    
    if(col == map->cols -1){
        return DIR_RIGHT;
    }
    
    return DIR_NONE;
}


// contents of map.c
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

bool mapIsComplete(map_t* map){
    unsigned int r;
    unsigned int c;
    
    for(r = 0; r < map->rows; r++){
        for(c = 0; c < map->cols; c++){
            if(mapGet(map, r, c) == FIELD_UNKNOWN){
                return false;
            }
        }
    }
    
    return true;
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

void mapCompleteField(
    map_t* dest, map_t* src, unsigned int row, unsigned int col
){
    if(
        mapGet(dest, row, col) == FIELD_UNKNOWN
        && (
            mapIsObstacle(dest, row - 1, col)
            || mapIsObstacle(dest, row + 1, col)
        )
        && (
            mapIsObstacle(dest, row, col - 1)
            || mapIsObstacle(dest, row, col + 1)
        )
    ){
        mapSet(dest, row, col, FIELD_OBSTACLE);
        
        mapCompleteField(dest, src, row - 1, col);
        mapCompleteField(dest, src, row + 1, col);
        mapCompleteField(dest, src, row, col - 1);
        mapCompleteField(dest, src, row, col + 1);
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

unsigned int mapFieldsAnalyzed(map_t* map){
    unsigned int r;
    unsigned int c;
    unsigned analyzed = 0;
    
    for(r = 0; r < map->rows; r++){
        for(c = 0; c < map->cols; c++){
            switch(mapGet(map, r, c)){
                case FIELD_UNKNOWN:
                    break;
                default:
                    analyzed++;
            }
        }
    }
    
    return analyzed;
}

unsigned int mapFieldsNotObst(map_t* map){
    unsigned int r;
    unsigned int c;
    unsigned notObsts = 0;
    
    for(r = 0; r < map->rows; r++){
        for(c = 0; c < map->cols; c++){
            switch(mapGet(map, r, c)){
                case FIELD_UNKNOWN:
                case FIELD_OBSTACLE:
                    break;
                default:
                    notObsts++;
            }
        }
    }
    
    return notObsts;
}


// contents of obstacle.c
void obstPrint(obst_t* obst){
    printf("%u-%u ", obst->top, obst->bottom);
    printf("%u-%u\n", obst->left, obst->right);
}

bool obstInMap(obst_t* obst, map_t* map){
    if(
       pointInMap(obst->top, obst->left, map)
       && pointInMap(obst->bottom, obst->right, map)
       ){
        return true;
    }else{
        return false;
    }
}

bool obstOnBorder(obst_t* obst, map_t* map){
    if(
       pointOnBorder(obst->top, obst->left, map)
       || pointOnBorder(obst->bottom, obst->right, map)
       ){
        return true;
    }else{
        return false;
    }
}

void obstSetInit(obstSet_t* set){
    set->used = 0;
    set->set = (obst_t*) malloc(set->length * sizeof(obst_t));
}

int obstSetCheck(obstSet_t* set, obst_t* obst){
    unsigned int o;
    
    for(o = 0; o < set->used; o++){
        if(
           obst->top - 1 <= set->set[o].bottom
           && obst->bottom + 1 >= set->set[o].top
           && obst->left - 1 <= set->set[o].right
           && obst->right + 1 >= set->set[o].left
           ){
            return o;
        }
    }
    
    return -1;
}

void obstSetAdd(obstSet_t* set, obst_t* obst){
    if(set->used < set->length){
        set->set[set->used].top = obst->top;
        set->set[set->used].bottom = obst->bottom;
        set->set[set->used].left = obst->left;
        set->set[set->used].right = obst->right;
        set->used++;
    }
}

void obstSetFree(obstSet_t* set){
    free(set->set);
    set->set = NULL;
}

// contents of robot.c
void robInit(rob_t* rob){
    rob->active = false;
    rob->explored = false;
}

void robSpawn(terra_t* env){
    rob_t* rob = &env->robs.set[env->robs.spawned];
    
    rob->active = true;
    rob->row = env->accessRow;
    rob->col = env->accessCol;
    rob->mode = MODE_PREPARE;
    
    mapSet(&env->robs.map, rob->row, rob->col, FIELD_ROBOT);
    mapExplore(&env->robs.map, &env->map, rob->row, rob->col);
    
    env->robs.active++;
    env->robs.spawned++;
}

void robReturn(rob_t* rob, terra_t* env){
    if(rob->explored){
        return;
    }
    
    rob->explored = true;
    switch(rob->globalDir){
        case DIR_TOP:
            rob->dist = env->accessRow;
            rob->globalDir = DIR_BOTTOM;
            break;
            
        case DIR_BOTTOM:
            rob->dist = 0;
            rob->globalDir = DIR_TOP;
            break;
            
        case DIR_LEFT:
            rob->dist = env->accessCol;
            rob->globalDir = DIR_RIGHT;
            break;
            
        case DIR_RIGHT:
            rob->dist = 0;
            rob->globalDir = DIR_LEFT;
            break;
    }
    
}

void robEvac(rob_t* rob, terra_t* env){
    rob->active = false;
    env->robs.active--;
    
    mapSet(&env->robs.map, env->accessRow, env->accessCol, FIELD_ACCESS);
}

char robThinkPrepare(rob_t* rob, terra_t* env){
    switch(rob->globalDir){
        case DIR_TOP:
        case DIR_BOTTOM:
            if(rob->col > env->plan.start && robCanMove(rob, DIR_LEFT, env)){
                return DIR_LEFT;
            }
            
            if(rob->col < env->plan.start && robCanMove(rob, DIR_RIGHT, env)){
                return DIR_RIGHT;
            }
            
            if(rob->col == env->plan.start){
                return robThinkExplore(rob, env);
            }
            
            break;
            
        case DIR_LEFT:
        case DIR_RIGHT:
            if(rob->row > env->plan.start && robCanMove(rob, DIR_TOP, env)){
                return DIR_TOP;
            }
            
            if(rob->row < env->plan.start && robCanMove(rob, DIR_BOTTOM, env)){
                return DIR_BOTTOM;
            }
            
            if(rob->row == env->plan.start){
                return robThinkExplore(rob, env);
            }
            
            break;
    }
    
    // in case we end up here:
    // we're not on correct row but
    // we can't move either
    //
    // What we do?
    return robThinkExplore(rob, env);
}

char robThinkExplore(rob_t* rob, terra_t* env){
    rob->mode = MODE_EXPLORE;
    
    // go to next row / col when done
    bool next = false;
    
    switch(rob->dir){
        case DIR_TOP:
            if(rob->row == 0){
                next = true;
                rob->dir = DIR_BOTTOM;
            }
            
            break;
            
        case DIR_BOTTOM:
            if(rob->row == env->map.rows - 1){
                next = true;
                rob->dir = DIR_TOP;
            }
            
            break;
            
        case DIR_LEFT:
            if(rob->col == 0){
                next = true;
                rob->dir = DIR_RIGHT;
            }
            
            break;
            
        case DIR_RIGHT:
            if(rob->col == env->map.cols - 1){
                next = true;
                rob->dir = DIR_LEFT;
            }
            
            break;
    }
    
    if(next){
        switch(rob->globalDir){
            case DIR_TOP:
                if(rob->row >= env->plan.dist){
                    rob->dist -= env->plan.dist;
                }else if(!rob->explored){
                    robReturn(rob, env);
                }
                break;
                
            case DIR_BOTTOM:
                if(env->map.rows - rob->row > env->plan.dist){
                    rob->dist += env->plan.dist;
                }else if(!rob->explored){
                    robReturn(rob, env);
                }
                break;
                
            case DIR_LEFT:
                if(rob->col >= env->plan.dist){
                    rob->dist -= env->plan.dist;
                }else if(!rob->explored){
                    robReturn(rob, env);
                }
                break;
                
            case DIR_RIGHT:
                if(env->map.cols - rob->col > env->plan.dist){
                    rob->dist += env->plan.dist;
                }else if(!rob->explored){
                    robReturn(rob, env);
                }
                break;
        }
    }
    
    switch(rob->globalDir){
        case DIR_TOP:
        case DIR_BOTTOM:
            
            // correct row if needed and possible
            if(rob->row > rob->dist && robCanMove(rob, DIR_TOP, env)){
                return DIR_TOP;
            }
            
            if(rob->row < rob->dist && robCanMove(rob, DIR_BOTTOM, env)){
                return DIR_BOTTOM;
            }
            
            // assume line correct
            switch(rob->dir) {
                case DIR_LEFT:
                    if(robCanMove(rob, DIR_LEFT, env)){
                        return DIR_LEFT;
                    }
                    break;
                    
                case DIR_RIGHT:
                    if(robCanMove(rob, DIR_RIGHT, env)){
                        return DIR_RIGHT;
                    }
                    break;
            }
            
            break;
            
        case DIR_LEFT:
        case DIR_RIGHT:
            if(rob->col > rob->dist && robCanMove(rob, DIR_LEFT, env)){
                return DIR_LEFT;
            }
            
            if(rob->col < rob->dist && robCanMove(rob, DIR_RIGHT, env)){
                return DIR_RIGHT;
            }
            
            switch(rob->dir){
                case DIR_TOP:
                    if(robCanMove(rob, DIR_TOP, env)){
                        return DIR_TOP;
                    }
                    break;
                    
                case DIR_BOTTOM:
                    if(robCanMove(rob, DIR_BOTTOM, env)){
                        return DIR_BOTTOM;
                    }
                    break;
            }
            
            break;
    }
    
    return robThinkAvoid(rob, env);
}

char robThinkAvoid(rob_t* rob, terra_t* env){
    rob->mode = MODE_AVOID;
    
    switch(rob->dir){
        case DIR_TOP:
            if(robCanMove(rob, DIR_TOP, env)){
                rob->mode = MODE_EXPLORE;
                return DIR_TOP;
            }
            break;
            
        case DIR_BOTTOM:
            if(robCanMove(rob, DIR_BOTTOM, env)){
                rob->mode = MODE_EXPLORE;
                return DIR_BOTTOM;
            }
            break;
            
        case DIR_LEFT:
            if(robCanMove(rob, DIR_LEFT, env)){
                rob->mode = MODE_EXPLORE;
                return DIR_LEFT;
            }
            break;
            
        case DIR_RIGHT:
            if(robCanMove(rob, DIR_RIGHT, env)){
                rob->mode = MODE_EXPLORE;
                return DIR_RIGHT;
            }
            break;
    }
    
    switch(rob->globalDir) {
        case DIR_TOP:
            if(robCanMove(rob, DIR_BOTTOM, env)){
                return DIR_BOTTOM;
            }else if(robCanMove(rob, DIR_TOP, env)){
                return DIR_TOP;
            }
            break;
            
        case DIR_BOTTOM:
            if(robCanMove(rob, DIR_TOP, env)){
                return DIR_TOP;
            }else if(robCanMove(rob, DIR_BOTTOM, env)){
                return DIR_BOTTOM;
            }
            break;
            
        case DIR_LEFT:
            if(robCanMove(rob, DIR_RIGHT, env)){
                return DIR_RIGHT;
            }else if(robCanMove(rob, DIR_LEFT, env)){
                return DIR_LEFT;
            }
            break;
            
        case DIR_RIGHT:
            if(robCanMove(rob, DIR_LEFT, env)){
                return DIR_LEFT;
            }else if(robCanMove(rob, DIR_RIGHT, env)){
                return DIR_RIGHT;
            }
            break;
    }
    
    return DIR_NONE;
}

char robThink(rob_t* rob, terra_t* env){
    if(
       rob->explored
       && rob->row == env->accessRow
       && rob->col == env->accessCol
       ){
        robEvac(rob, env);
        return DIR_NONE;
    }
    
    switch(rob->mode){
        case MODE_PREPARE:
            return robThinkPrepare(rob, env);
            
        case MODE_EXPLORE:
            return robThinkExplore(rob, env);
            
        case MODE_AVOID:
            return robThinkAvoid(rob, env);
    }
    
    return DIR_NONE;
}

void robMove(rob_t* rob, char dir, terra_t* env){
    unsigned int newRow = rob->row;
    unsigned int newCol = rob->col;
    
    switch(dir){
        case DIR_TOP:
            newRow--;
            break;
            
        case DIR_BOTTOM:
            newRow++;
            break;
            
        case DIR_LEFT:
            newCol--;
            break;
            
        case DIR_RIGHT:
            newCol++;
            break;
            
        default:
            return;
    }
    
    // explore area
    mapExplore(&env->robs.map, &env->map, newRow, newCol);
    
    // map old robot position
    mapSet(
           &env->robs.map, rob->row, rob->col,
           mapGet(&env->map, rob->row, rob->col)
           );
    
    // map new robot position
    mapSet(&env->robs.map, newRow, newCol, FIELD_ROBOT);
    
    rob->row = newRow;
    rob->col = newCol;
}

bool robCanMove(rob_t* rob, char dir, terra_t* env){
    switch(dir){
        case DIR_TOP:
            if(
               rob->row > 0
               && mapIsEmpty(&env->robs.map, rob->row - 1, rob->col)
               ){
                return true;
            }
            break;
            
        case DIR_BOTTOM:
            if(
               rob->row < env->map.rows - 1
               && mapIsEmpty(&env->robs.map, rob->row + 1, rob->col)
               ){
                return true;
            }
            break;
            
        case DIR_LEFT:
            if(
               rob->col > 0
               && mapIsEmpty(&env->robs.map, rob->row, rob->col - 1)
               ){
                return true;
            }
            break;
            
        case DIR_RIGHT:
            if(
               rob->col < env->map.cols - 1
               && mapIsEmpty(&env->robs.map, rob->row, rob->col + 1)
               ){
                return true;
            }
            break;
    }
    
    return false;
}

void robSetInit(robSet_t* set){
    set->active = 0;
    set->spawned = 0;
    set->planned = 0;
    set->set = (rob_t*) malloc(set->length * sizeof(rob_t));
    
    unsigned int r;
    for(r = 0; r < set->length; r++){
        robInit(&set->set[r]);
    }
}

void robSetFree(robSet_t* set){
    free(set->set);
    set->set = NULL;
}


// contents of main.c
void emptyStdIn(void){
    while(getchar() != '\n');
}

void printPrompt(void){
    putchar('>');
    putchar(' ');
    fflush(stdout);
}

bool readDialogMode(terra_t* env){
    int in;
    
    if(scanf("%d", &in) < 1){
        printf("ERROR: Invalid input for dialog mode\n");
        return false;
    }
    
    if(in){
        env->dialogMode = false;
    }else{
        env->dialogMode = true;
    }
    
    if(env->dialogMode){
        printf(
            "< Dialog mode selected\n"
            "\n"
        );
    }
    
    emptyStdIn();
    
    return true;
}

bool readResultMode(terra_t* env){
    int in;
    
    if(env->dialogMode){
        printf(
            "SELECT RESULT MODE\n"
            "0\tComplete\n"
            "1\tRedirection\n"
        );
        printPrompt();
    }
    
    if(scanf("%d", &in) < 1){
        printf("ERROR: Invalid input for result mode\n");
        return false;
    }
    
    if(in){
        env->resultMode = false;
        
        if(env->dialogMode){
            printf(
                "< Redirection mode selected\n"
                "\n"
            );
        }
    }else{
        env->resultMode = true;
        
        if(env->dialogMode){
            printf(
                "< Complete mode selected\n"
                "\n"
            );
        }
    }
    
    emptyStdIn();
    
    return true;
}

bool readMapSize(terra_t* env){
    int rows;
    int cols;
    
    if(env->dialogMode){
        printf("ENTER MAP SIZE\n");
        printPrompt();
    }
    
    if(scanf("%d %d", &rows, &cols) < 2){
        printf("ERROR: Invalid input for map size\n");
        return false;
    }
    
    if(rows < 1 || cols < 1){
        printf("ERROR: Map size is invalid\n");
        return false;
    }
    
    env->map.rows = rows;
    env->map.cols = cols;
    mapInit(&env->map, FIELD_EMPTY);
    
    env->robs.map.rows = rows;
    env->robs.map.cols = cols;
    mapInit(&env->robs.map, FIELD_UNKNOWN);
    
    if(env->dialogMode){
        printf(
            "< Map size: %u %u\n"
            "\n",
            env->map.rows, env->map.cols
        );
    }
    
    emptyStdIn();
    
    return true;
}

bool readAccessPoint(terra_t* env){
    int rows;
    int cols;
    
    if(env->dialogMode){
        printf("ENTER POSITION OF ACCESS POINT\n");
        printPrompt();
    }
    
    if(scanf("%d %d", &rows, &cols) < 2){
        printf("ERROR: Invalid input for access point\n");
        return false;
    }
    
    if(rows < 0 || cols < 0){
        printf("ERROR: Negative coordinate\n");
        return false;
    }
    
    if(!pointInMap(rows, cols, &env->map)){
        printf("ERROR: Access point out of bounds\n");
        return false;
    }
    
    if(!pointOnBorder(rows, cols, &env->map)){
        printf("ERROR: Access point not on border\n");
        return false;
    }
    
    env->accessRow = rows;
    env->accessCol = cols;
    
    // add access point to map
    mapSet(&env->map, env->accessRow, env->accessCol, FIELD_ACCESS);
    
    if(env->dialogMode){
        printf(
            "< Position of access point: %u %u\n"
            "\n",
            env->accessRow,
            env->accessCol
        );
    }
    
    emptyStdIn();
    
    return true;
}

bool readNumbRobs(terra_t* env){
    int numb;
    
    if(env->dialogMode){
        printf("ENTER NUMBER OF ROBOTS\n");
        printPrompt();
    }
    
    if(scanf("%d", &numb) < 1){
        printf("ERROR: Invalid input for number of robots\n");
        return false;
    }
    
    if(numb < 1){
        printf("ERROR: Invalid number of robots\n");
        return false;
    }
    
    env->robs.length = numb;
    robSetInit(&env->robs);
    
    if(env->dialogMode){
        printf(
            "< Number of robots: %u\n"
            "\n",
            env->robs.length
        );
    }
    
    emptyStdIn();
    
    return true;
}

bool readNumbObsts(terra_t* env){
    int numb;
    
    if(env->dialogMode){
        printf("ENTER NUMBER OF OBSTACLES\n");
        printPrompt();
    }
    
    if(scanf("%d", &numb) < 1){
        printf("ERROR: Invalid input for number of obstacles\n");
        return false;
    }
    
    if(numb < 0){
        printf("ERROR: Invalid number of obstacles\n");
        return false;
    }
    
    env->obsts.length = numb;
    obstSetInit(&env->obsts);
    
    if(env->dialogMode){
        printf(
            "< Number of obstacles: %u\n"
            "\n",
            env->obsts.length
        );
    }
    
    emptyStdIn();
    
    return true;
}

bool readObsts(terra_t* env){
    obst_t obst;
    int coll;
    unsigned int o;
    
    if(!env->obsts.length){
        return true;
    }
    
    if(env->dialogMode){
        printf("ENTER POSITIONS OF OBSTACLES\n");
        printPrompt();
    }
    
    for(o = 0; o < env->obsts.length; o++){
        if(!readCoord(&obst.top) || !readCoord(&obst.left)){
            return false;
        }
        
        if(!obstInMap(&obst, &env->map)){
            printf("ERROR: Obstacle %u not in map\n", o);
            return false;
        }
        
        if(obstOnBorder(&obst, &env->map)){
            printf("ERROR: Obstacle %u on border\n", o);
            return false;
        }
        
        if((coll = obstSetCheck(&env->obsts, &obst)) >= 0){
            printf("ERROR: Obstacles %u and %u are too close\n", coll, o);
            
            printf("Obst %u: ", coll);
            obstPrint(&env->obsts.set[coll]);
            
            printf("Obst %u: ", o);
            obstPrint(&obst);
            return false;
        }
        
        obstSetAdd(&env->obsts, &obst);
        mapAddObstacle(&env->map, &obst);
        
        if(env->dialogMode){
            printf(
                "< Position of obstacle %u: %u-%u %u-%u\n",
                o, obst.top, obst.bottom, obst.left, obst.right
            );
        }
    }
    
    if(env->dialogMode){
        printf("\n");
    }
    
    return true;
}

bool readCoord(unsigned int* coord){
    int start;
    int end;
    
    if(scanf("%d", &start) < 1){
        printf("ERROR: Invalid input for obstacle coordinates\n");
        return false;
    }
    
    if(getchar() == '-'){
        // range notation
        if(scanf("%d", &end) < 1){
            printf("ERROR: Invalid input for obstacle coordinates\n");
            return false;
        }
    }else{
        // isolated notation
        end = start;
    }
    
    if(start < 0 || end < 0){
        printf("ERROR: Negative coordinate\n");
    }
    
    if(end < start){
        coord[0] = end;
        coord[1] = start;
        printf("NOTICE: Automatically corrected coordinate order\n");
    }else{
        coord[0] = start;
        coord[1] = end;
    }
    
    return true;
}

bool readData(terra_t* env){
    bool ok = true;
    ok = ok && readDialogMode(env);
    ok = ok && readResultMode(env);
    ok = ok && readMapSize(env);
    ok = ok && readAccessPoint(env);
    ok = ok && readNumbRobs(env);
    ok = ok && readNumbObsts(env);
    ok = ok && readObsts(env);
    
    return ok;
}

void plan(terra_t* env){
    char globalDir;
    unsigned int r;
    unsigned int dir;
    unsigned int dim;
    unsigned int access;
    unsigned int accessDim;
    unsigned int numbRobs;
    
    switch(pointGetBorder(env->accessRow, env->accessCol, &env->map)){
        case DIR_TOP:
            dim = env->map.rows;
            access = env->accessCol;
            accessDim = env->map.cols;
            globalDir = DIR_BOTTOM;
            break;
            
        case DIR_BOTTOM:
            dim = env->map.rows;
            access = env->accessCol;
            accessDim = env->map.cols;
            globalDir = DIR_TOP;
            break;
            
        case DIR_LEFT:
            dim = env->map.cols;
            access = env->accessRow;
            accessDim = env->map.rows;
            globalDir = DIR_RIGHT;
            break;
            
        case DIR_RIGHT:
            dim = env->map.cols;
            access = env->accessRow;
            accessDim = env->map.rows;
            globalDir = DIR_LEFT;
            break;
            
        default:
            return;
    }
    
    numbRobs = dim / 2;
    
    // not enough robots?
    if(numbRobs > env->robs.length){
        env->plan.numbRobs = env->robs.length;
    }else{
        env->plan.numbRobs = numbRobs;
    }
    
    // calculate start position ...
    if(accessDim - access - 1 > access){
        env->plan.start = 0;
        
        // ... and direction 
        switch(globalDir){
            case DIR_TOP:
            case DIR_BOTTOM:
                dir = DIR_RIGHT;
                break;
            
            case DIR_LEFT:
            case DIR_RIGHT:
                dir = DIR_BOTTOM;
                break;
        }
    }else{
        env->plan.start = accessDim - 1;
        
        switch(globalDir){
            case DIR_TOP:
            case DIR_BOTTOM:
                dir = DIR_LEFT;
                break;
                
            case DIR_LEFT:
            case DIR_RIGHT:
                dir = DIR_TOP;
                break;
        }
    }
    
    // calculate rob position
    for(r = 0; r < env->plan.numbRobs; r++){
        env->robs.set[r].dir = dir;
        env->robs.set[r].globalDir = globalDir;
        
        switch(globalDir){
            case DIR_TOP:
            case DIR_LEFT:
                env->robs.set[r].dist = dim - 2 * (env->plan.numbRobs - r);
                break;
                
            case DIR_BOTTOM:
            case DIR_RIGHT:
                env->robs.set[r].dist = 2 * (env->plan.numbRobs - r) -1 ;
                break;
        }
    }
    
    // dist to next row / col of robot
    env->plan.dist = 2 * env->plan.numbRobs;
}

bool init(terra_t* env){
    env->map.map = NULL;
    env->robs.map.map = NULL;
    env->obsts.set = NULL;
    
    if(!readData(env)){
        return false;
    }
    
    plan(env);
    mapExploreBorder(&env->robs.map, &env->map);
    
    if(env->resultMode){
        mapPrint(&env->map);
    }
    
    return true;
}

void loop(terra_t* env){
    rob_t* rob = NULL;
    bool done = false;
    char result = SUCCESS;
    unsigned int numbCycles = 0;
    unsigned int limitCycles = env->map.length;
    
    while(!done && numbCycles < limitCycles){
        done = true;
        numbCycles++;
        
        unsigned int r;
        for(r = 0; r < env->plan.numbRobs; r++){
            rob = &env->robs.set[r];
            
            if(rob->active){
                robMove(rob, robThink(rob, env), env);
            }
        }
        
        if(
           env->robs.spawned < env->plan.numbRobs
           && mapIsEmpty(&env->robs.map, env->accessRow, env->accessCol)
        ){
            robSpawn(env);
        }
        
        mapComplete(&env->robs.map, &env->map);
        
        if(env->resultMode){
            mapPrint(&env->robs.map);
        }
        
        if(env->robs.active){
            done = false;
        }
        
        if(numbCycles == env->map.length){
            if(mapIsComplete(&env->robs.map)){
                limitCycles = 2 * env->map.length;
                
                for(r = 0; r < env->plan.numbRobs; r++){
                    robReturn(&env->robs.set[r], env);
                }
            }else{
                done = true;
                result = CONSTR_TIMEOUT;
            }
        }
        
        if(numbCycles == 2 * env->map.length){
            if(env->robs.active){
                done = true;
                result = EVAC_TIMEOUT;
            }
        }
    }
    
    env->result = result;
    env->numbCycles = numbCycles;
}

void stats(terra_t* env){
    if(!env->resultMode){
        mapPrint(&env->robs.map);
    }
    
    printf("\n");
    
    switch(env->result){
        case SUCCESS:
            printf("> SUCCESS\n");
            break;
        case CONSTR_TIMEOUT:
            printf("> CONSTRUCTION TIMEOUT\n");
            break;
        case EVAC_TIMEOUT:
            printf("> EVACUATION TIMEOUT\n");
            break;
    }
    
    printf("\n");
    printf("> Number of robots: %d\n", env->plan.numbRobs);
    printf("> Number of cycles: %d\n", env->numbCycles);
    printf("\n");
    printf(
        "%u / %u / %u\n",
        mapFieldsAnalyzed(&env->robs.map),
        mapFieldsNotObst(&env->robs.map),
        env->robs.map.length
    );
}
    


void clean(terra_t* env){
    mapFree(&env->map);
    mapFree(&env->robs.map);
    obstSetFree(&env->obsts);
}

int main(void){
    terra_t env;
    
    if(init(&env)){
        loop(&env);
        stats(&env);
    }
    
    clean(&env);
    
    return EXIT_SUCCESS;
}