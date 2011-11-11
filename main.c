#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "map.h"
#include "point.h"
#include "obstacle.h"

static int dialogMode;
static int displayMode;

static map_t map;
static point_t accessPoint;

// TODO
// Create rob set structure
static unsigned int numbRobs;

static obstSet_t obstSet;

void emptyStdIn(){
    while(getchar() != '\n');
}

void printPrompt(){
    putchar('>');
    putchar(' ');
    fflush(stdout);
}

bool readDialogMode(){
    printf("SELECT DIALOG MODE\n");
    printf("0\tDialog\n");
    printf("1\tRedirection\n");
    printPrompt();
    
    int in = 0;
    if(scanf("%d", &in) < 1){
        printf("ERROR: Invalid input\n");
        return false;
    }
    
    if(in){
        dialogMode = MODE_REDIRECT;
    }else{
        dialogMode = MODE_CONSOLE;
    }
    
    emptyStdIn();
    printf("\n");
    
    return true;
}

bool readDisplayMode(){
    printf("SELECT DISPLAY MODE\n");
    printf("0\tComplete\n");
    printf("1\tRedirection\n");
    printPrompt();
    
    int in = 0;
    if(scanf("%d", &in) < 1){
        printf("ERROR: Invalid input\n");
        return false;
    }
    
    if(in){
        displayMode = MODE_REDIRECT;
    }else{
        displayMode = MODE_CONSOLE;
    }
    
    emptyStdIn();
    printf("\n");
    
    return true;
}

bool readMapSize(){
    int rows = 0;
    int cols = 0; 
    
    printf("ENTER MAP SIZE\n");
    printPrompt(); 
    
    if(scanf("%d %d", &rows, &cols) < 2){
        printf("ERROR: Invalid input\n");
        return false;
    }
    
    if(rows < 0 || cols < 0){
        printf("ERROR: Map size is invalid\n");
        return false;
    }
    
    map.rows = rows;
    map.cols = cols;
    mapInit(&map, FIELD_EMPTY);
    
    emptyStdIn();
    printf("\n");
    
    return true;
}

bool readAccessPoint(){
    int rows = 0;
    int cols = 0;
    
    printf("ENTER POSITION OF ACCESS POINT\n");
    printPrompt();
    
    if(scanf("%d %d", &rows, &cols) < 2){
        printf("ERROR: Invalid input\n");
        return false;
    }
    
    if(rows < 0 || cols < 0){
        printf("ERROR: Negative coordinate\n");
        return false;
    }
    
    accessPoint.row = rows;
    accessPoint.col = cols;
    
    if(!pointInMap(&accessPoint, &map)){
        printf("ERROR: Access point out of bounds\n");
        return false;
    }
    
    mapSetAccessPoint(&map, &accessPoint);
    
    emptyStdIn();
    printf("\n");
    
    return true;
}

bool readNumbRobs(){
    int numb;
    
    printf("ENTER NUMBER OF ROBOTS\n");
    printPrompt();
    
    if(scanf("%d", &numb) < 1){
        printf("ERROR: Invalid input\n");
        return false;
    }
    
    if(numb < 1){
        printf("ERROR: Number of robots is invalid\n");
        return false;
    }
    
    numbRobs = numb;
    
    // TODO
    // robsInit(robSet)
    
    emptyStdIn();
    printf("\n");
    
    return true;
}

bool readNumbObsts(){
    int numb;
    
    printf("ENTER NUMBER OF OBSTACLES\n");
    printPrompt();
    
    if(scanf("%d", &numb) < 1){
        printf("ERROR: Invalid input\n");
        return false;
    }
    
    if(numb < 0){
        printf("ERROR: Invalid number of obstacles\n");
        return false;
    }
    
    obstSet.length = numb;
    obstSetInit(&obstSet);
    
    emptyStdIn();
    printf("\n");
    
    return true;
}

bool readObsts(){
    obst_t obst;
    unsigned int o;
    unsigned int i;
    
    printf("ENTER POSITIONS OF OBSTACLES\n");
    
    for(o = 0; o < obstSet.length; o++){
        if(!readCoord(&(obst.top))){
            return false;
        }
           
        if(!readCoord(&(obst.left))){
            return false;
        }
        
        if(!obstInMap(&obst, &map)){
            printf("ERROR: Obstacles not in map\n");
            return false;
        }
        
        if(obstOnBorder(&obst, &map)){
            printf("ERROR: Obstacles on border\n");
            return false;
        }
        
        if(!obstIsSeparate(&obst, &obstSet)){
            printf("ERROR: Not enough space between obstacles\n");
            return false;
        }
        
        obstSetAdd(&obstSet, &obst);
    }
    
    mapAddObstacles(&map, &obstSet);
    
    emptyStdIn();
    printf("\n");
    
    return true;
}

bool readCoord(unsigned int* coord){
    int start;
    int end;
    
    if(scanf("%d", &start) < 1){
        printf("ERROR: Invalid input\n");
        return false;
    }
    
    if(getchar() == '-'){
        // range notation
        if(scanf("%d", &end) < 1){
            printf("ERROR: Invalid input\n");
            return false;
        }
    }else{
        // isolated notation
        end = start;
    }
    
    if(start < 0 || end < 0){
        printf("ERROR: Negative coordinate\n");
    }
    
    coord[0] = start;
    coord[1] = end;
    
    return true;
}

bool readData(){
    bool ok = true;
    ok = ok && readDialogMode();
    ok = ok && readDisplayMode();
    ok = ok && readMapSize();
    ok = ok && readAccessPoint();
    ok = ok && readNumbRobs();
    ok = ok && readNumbObsts();
    ok = ok && readObsts();
    
    return ok;
}

void init(){
    bool ok = true;
    
    map.map = NULL;
    obstSet.set = NULL;

    ok = ok && readData();
    
    if(ok){
        mapPrint(&map);
    }
}

void loop(){
    // TODO
}

void clean(){
    mapFree(&map);
    obstSetFree(&obstSet);
}

int main(){
    init();
    loop();
    clean();
    
    return EXIT_SUCCESS;
}