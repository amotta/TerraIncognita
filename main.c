// TERRA INCOGNITA
// Rendu intermédiaire
// 17. 11. 2011
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


// contents of map.h
void mapInit(map_t* map, char fill);
void mapFree(map_t* map);
char mapGet(map_t* map, unsigned int row, unsigned int col);
void mapSet(map_t* map, unsigned int row, unsigned int col, char fill);
void mapAddObstacle(map_t* mall, obst_t* obst);
void mapPrint(map_t* map);


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
bool pointOnBorder(unsigned int row, unsigned int col, map_t* map);


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
bool readCoord(unsigned int* dim);
bool readData(terra_t* env);
void init(terra_t* env);
void loop(void);
void clean(terra_t* env);
int main(void);


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
           obst->top - 1 <= set->set[o].top
           && obst->bottom + 1 >= set->set[o].bottom
           && obst->left - 1 <= set->set[o].left
           && obst->right + 1 >= set->set[o].right
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
}


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
    
    if(env->dialogMode){
        printf(
            "< Map size: %u %u\n"
            "\n",
            env->map.rows,
            env->map.cols
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
    
    env->numbRobs = numb;
    
    // TODO
    // robsInit(robSet)
    
    if(env->dialogMode){
        printf(
            "< Number of robots: %u\n"
            "\n",
            env->numbRobs
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

void init(terra_t* env){
    env->map.map = NULL;
    env->obsts.set = NULL;
    
    if(!readData(env)){
        return;
    }
    
    if(env->resultMode){
        mapPrint(&env->map);
    }
}

void loop(void){
    // TODO
}

void clean(terra_t* env){
    mapFree(&env->map);
    obstSetFree(&env->obsts);
}

int main(void){
    terra_t env;
    
    init(&env);
    loop();
    clean(&env);
    
    return EXIT_SUCCESS;
}