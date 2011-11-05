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
    scanf("%d", &in);
    
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
    scanf("%d", &in);
    
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
    printf("ENTER MAP SIZE\n");
    printPrompt();
    
    unsigned int rows = 0;
    unsigned int cols = 0;
    scanf("%u %u", &rows, &cols);
    
    emptyStdIn();
    printf("\n");
    
    if(rows > 0 && cols > 0){
        map.rows = rows;
        map.cols = cols;
        mapInit(&map, FIELD_EMPTY);
        
        return true;
    }else{
        printf("ERROR: Map size is invalid\n");
        return false;
    }
}

bool readAccessPoint(){
    printf("ENTER POSITION OF ACCESS POINT\n");
    printPrompt();
    
    scanf("%u %u", &accessPoint.row, &accessPoint.col);
    
    emptyStdIn();
    printf("\n");
    
    if(pointInMap(&accessPoint, &map) && pointOnBorder(&accessPoint, &map)){
        mapSetAccessPoint(&map, &accessPoint);
        return true;
    }else{
        printf("ERROR: Position of access field is invalid\n");
        return false;
    }
}

bool readNumbRobs(){
    printf("ENTER NUMBER OF ROBOTS\n");
    printPrompt();
    
    unsigned int numb = 0;
    scanf("%u", &numb);
    
    emptyStdIn();
    printf("\n");
    
    if(numb > 0){
        numbRobs = numb;
        
        // TODO
        // robsInit(robSet)
        
        return true;
    }else{
        printf("ERROR: Number of robots is invalid\n");
        return false;
    }
}

bool readNumbObsts(){
    printf("ENTER NUMBER OF OBSTACLES\n");
    printPrompt();
    
    scanf("%u", &obstSet.length);
    obstSetInit(&obstSet);
    
    emptyStdIn();
    printf("\n");
    
    return true;
}

bool readObsts(){
    bool ok = true;
    unsigned int o;
    unsigned int i;
    
    obst_t obst;
    unsigned int coord[2][2];
    
    printf("ENTER POSITIONS OF OBSTACLES\n");
    
    for(o = 0; ok && o < obstSet.length; o++){
        for(i = 0; ok && i < 2; i++){
            ok = ok && readCoord(coord[i]);
        }
        
        obst.topLeft.row = coord[0][0];
        obst.topLeft.col = coord[1][0];
        obst.bottomRight.row = coord[0][1];
        obst.bottomRight.col = coord[1][1];
        
        ok = ok && obstInMap(&obst, &map);
        if(!ok){
            printf("ERROR: Obstacles is not in map\n");
            break;
        }
        
        ok = ok && !obstOnBorder(&obst, &map);
        if(!ok){
            printf("ERROR: Obstacles is on border\n");
            break;
        }
        
        ok = ok && obstIsSeparate(&obst, &obstSet);
        if(!ok){
            printf("ERROR: Not enough space to other obstacles\n");
            break;
        }
        
        // TODO
        // obstSetAdd(obst)
    }
    
    emptyStdIn();
    printf("\n");
    
    return ok;
}

bool readCoord(unsigned int* coord){
    bool ok = true;
    char buf[65] = {0};
    unsigned int start = 0;
    unsigned int end = 0;
    
    // get single coordinate
    if(scanf("%64s", buf) < 1){
        printf("ERROR: Coordinate too long\n");
        return false;
    }
    
    if(strchr(buf, '-') == NULL){
        // isolated notation
        if(sscanf(buf, "%u", &start) < 1){
            ok = false;
        }else{
            coord[0] = start;
            coord[1] = start;
        }
    }else{
        // range notation
        if(sscanf(buf, "%u-%u", &start, &end) < 2){
            ok = false;
        }else{
            // check order
            if(start < end){
                coord[0] = start;
                coord[1] = end;
            }else{
                printf("WARNING: Fixed coordinate order\n");
                coord[0] = end;
                coord[1] = start;
            }
        }
    }
    
    if(ok){
        return true;
    }else{
        printf("ERROR: Could not parse coordinate\n");
        return false;
    }
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
    map.map = NULL;
    obstSet.set = NULL;
    
    readData();
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