#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "map.h"
#include "obstacle.h"
#include "point.h"

void emptyStdIn(){
    while(getchar() != '\n');
}

void printPrompt(){
    putchar('>');
    putchar(' ');
    fflush(stdout);
}

bool readDialogMode(terra_t* env){
    printf("SELECT DIALOG MODE\n");
    printf("0\tDialog\n");
    printf("1\tRedirection\n");
    printPrompt();
    
    int in;
    if(scanf("%d", &in) < 1){
        printf("ERROR: Invalid input\n");
        return false;
    }
    
    if(in){
        env->dialogMode = MODE_REDIRECT;
    }else{
        env->dialogMode = MODE_CONSOLE;
    }
    
    emptyStdIn();
    printf("\n");
    
    return true;
}

bool readDisplayMode(terra_t* env){
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
        env->displayMode = MODE_REDIRECT;
    }else{
        env->displayMode = MODE_CONSOLE;
    }
    
    emptyStdIn();
    printf("\n");
    
    return true;
}

bool readMapSize(terra_t* env){
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
    
    env->map.rows = rows;
    env->map.cols = cols;
    mapInit(&env->map, FIELD_EMPTY);
    
    emptyStdIn();
    printf("\n");
    
    return true;
}

bool readAccessPoint(terra_t* env){
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
    
    env->accessRow = rows;
    env->accessCol = cols;
    
    if(!pointInMap(env->accessRow, env->accessCol, &env->map)){
        printf("ERROR: Access point out of bounds\n");
        return false;
    }
    
    if(!pointOnBorder(env->accessRow, env->accessCol, &env->map)){
        printf("ERROR: Access point not on border\n");
        return false;
    }
    
    // add access point to map
    mapSet(&env->map, env->accessRow, env->accessCol, FIELD_ACCESS);
    
    emptyStdIn();
    printf("\n");
    
    return true;
}

bool readNumbRobs(terra_t* env){
    int numb;
    
    printf("ENTER NUMBER OF ROBOTS\n");
    printPrompt();
    
    if(scanf("%d", &numb) < 1){
        printf("ERROR: Invalid input\n");
        return false;
    }
    
    if(numb < 1){
        printf("ERROR: Invalid number of robots\n");
        return false;
    }
    
    env->numbRobs = numb;
    
    // TODO
    // robsInit(robSet)
    
    emptyStdIn();
    printf("\n");
    
    return true;
}

bool readNumbObsts(terra_t* env){
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
    
    env->numbObsts = numb;
    
    emptyStdIn();
    printf("\n");
    
    return true;
}

bool readObsts(terra_t* env){
    obst_t obst;
    unsigned int o;
    
    if(!env->numbObsts){
        return true;
    }
    
    printf("ENTER POSITIONS OF OBSTACLES\n");
    for(o = 0; o < env->numbObsts; o++){
        if(!readCoord(&obst.top)){
            return false;
        }
           
        if(!readCoord(&obst.left)){
            return false;
        }
        
        if(!obstInMap(&obst, &env->map)){
            printf("ERROR: Obstacle not in map\n");
            return false;
        }
        
        if(obstOnBorder(&obst, &env->map)){
            printf("ERROR: Obstacle on border\n");
            return false;
        }
        
        if(obstCollides(&obst, &env->map)){
            printf("ERROR: Obstacles too close\n");
            return false;
        }
        
        mapAddObstacle(&env->map, &obst);
    }
    
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

bool readData(terra_t* env){
    bool ok = true;
    ok = ok && readDialogMode(env);
    ok = ok && readDisplayMode(env);
    ok = ok && readMapSize(env);
    ok = ok && readAccessPoint(env);
    ok = ok && readNumbRobs(env);
    ok = ok && readNumbObsts(env);
    ok = ok && readObsts(env);
    
    return ok;
}

void init(terra_t* env){
    if(!readData(env)){
        return;
    }
    
    mapPrint(&env->map);
}

void loop(){
    // TODO
}

void clean(terra_t* env){
    mapFree(&env->map);
}

int main(){
    terra_t env;
    
    init(&env);
    loop();
    clean(&env);
    
    return EXIT_SUCCESS;
}