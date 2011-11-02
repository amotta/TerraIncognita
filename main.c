#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "map.h"
#include "point.h"

static int dialogMode;
static int displayMode;

static map_t map;
static point_t accessPoint;

// TODO
// Create rob set structure
static unsigned int numbRobs;

// TODO
// same here
static unsigned int numbObs;

void emptyStdIn(){
    while(getchar() != '\n');
}

void printPrompt(){
    putchar('>');
    putchar(' ');
    fflush(stdout);
}

void readDialogMode(){
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
}

void readDisplayMode(){
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
        return true;
    }else{
        printf("ERROR: Map size is invalid\n");
        return false;
    }
}

bool readAccessPoint(){
    printf("ENTER POSITION OF ACCESS POINT\n");
    printPrompt();
    
    unsigned int row = 0;
    unsigned int col = 0;
    scanf("%u %u", &row, &col);
    
    emptyStdIn();
    printf("\n");
    
    if(
       // is position on map?
       row < map.rows && col < map.cols
       // is position on border?
       && (row == 0 || row == map.rows - 1 || col == 0 || col == map.cols - 1)
    ){
        accessPoint.row = row;
        accessPoint.col = col;
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
        return true;
    }else{
        printf("ERROR: Number of robots is invalid\n");
        return false;
    }
}

void readNumbObs(){
    printf("ENTER NUMBER OF OBSTACLES\n");
    printPrompt();
    
    scanf("%u", &numbObs);
    
    emptyStdIn();
    printf("\n");
}

bool readObs(){
    bool ok = true;
    unsigned int o = 0;
    unsigned int i = 0;
    
    unsigned int start;
    unsigned int end;
    
    char buf[65] = {0};
    
    printf("ENTER POSITIONS OF OBSTACLES\n");
    printPrompt();
    
    while(o < numbObs){
        start = 0;
        end = 0;
        
        // get single coordinate
        ok = ok && (scanf("%64s", buf) == 1);
        if(!ok){
            printf("ERROR: Coordinate too long\n");
            return false;
        }
        
        if(strchr(buf, '-') == NULL){
            // isolated notation
            ok = ok && (sscanf(buf, "%u", &start) == 1);
            end = start;
        }else{
            // range notation
            ok = ok && (sscanf(buf, "%u-%u", &start, &end) == 2);
            
        }
        
        if(!ok){
            printf("ERROR: Could not parse coordinate\n");
            return false;
        }
        
        // TODO
        // validate start / end
        // and then store them
        
        i++;
        if(i == 2){
            o++;
            i = 0;
        }
    }
    
    emptyStdIn();
    printf("\n");
    
    return ok;
}

bool readData(){
    bool ok = true;
    
    readDialogMode();
    readDisplayMode();
    
    if((ok = ok && readMapSize())){
        mapInit(&map, FIELD_EMPTY);
    }else{
        map.map = NULL;
    }
    
    if((ok = ok && readAccessPoint())){
        mapSetAccessPoint(&map, &accessPoint);
    }
    
    if((ok = ok && readNumbRobs())){
        // TODO
        // robsInit(robSet)
    }
    
    readNumbObs();
    // TODO
    // obsInit(obsSet)
    
    if((ok = ok && readObs())){
        // TODO
        // mapAddObs(obsSet)
    }
    
    return ok;
}

bool init(){
    bool ok = true;
    ok = ok && readData();
    return ok;
}

bool loop(){
    return true;
}

bool clean(){
    mapFree(&map);
    
    return true;
}

int main(){
    init();
    loop();
    clean();
    return EXIT_SUCCESS;
}