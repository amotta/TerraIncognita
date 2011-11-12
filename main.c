#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "map.h"
#include "obstacle.h"
#include "point.h"

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
        ERROR("Invalid input\n")
        return false;
    }
    
    if(in){
        env->dialogMode = MODE_REDIRECT;
    }else{
        env->dialogMode = MODE_CONSOLE;
    }
    
    emptyStdIn();
    DIALOG("\n")
    
    return true;
}

bool readDisplayMode(terra_t* env){
    DIALOG(
        "SELECT DISPLAY MODE\n"
        "0\tComplete\n"
        "1\tRedirection\n"
    )
    PROMPT()
    
    int in = 0;
    if(scanf("%d", &in) < 1){
        ERROR("Invalid input\n")
        return false;
    }
    
    if(in){
        env->displayMode = MODE_REDIRECT;
    }else{
        env->displayMode = MODE_CONSOLE;
    }
    
    emptyStdIn();
    DIALOG("\n")
    
    return true;
}

bool readMapSize(terra_t* env){
    int rows = 0;
    int cols = 0; 
    
    DIALOG("ENTER MAP SIZE\n")
    PROMPT()
    
    if(scanf("%d %d", &rows, &cols) < 2){
        ERROR("Invalid input\n")
        return false;
    }
    
    if(rows < 0 || cols < 0){
        ERROR("Map size is invalid\n")
        return false;
    }
    
    env->map.rows = rows;
    env->map.cols = cols;
    mapInit(&env->map, FIELD_EMPTY);
    
    emptyStdIn();
    DIALOG("\n")
    
    return true;
}

bool readAccessPoint(terra_t* env){
    int rows = 0;
    int cols = 0;
    
    DIALOG("ENTER POSITION OF ACCESS POINT\n")
    PROMPT()
    
    if(scanf("%d %d", &rows, &cols) < 2){
        ERROR("Invalid input\n")
        return false;
    }
    
    if(rows < 0 || cols < 0){
        ERROR("Negative coordinate\n")
        return false;
    }
    
    env->accessRow = rows;
    env->accessCol = cols;
    
    if(!pointInMap(env->accessRow, env->accessCol, &env->map)){
        ERROR("Access point out of bounds\n")
        return false;
    }
    
    if(!pointOnBorder(env->accessRow, env->accessCol, &env->map)){
        ERROR("Access point not on border\n")
        return false;
    }
    
    // add access point to map
    mapSet(&env->map, env->accessRow, env->accessCol, FIELD_ACCESS);
    
    emptyStdIn();
    DIALOG("\n")
    
    return true;
}

bool readNumbRobs(terra_t* env){
    int numb;
    
    DIALOG("ENTER NUMBER OF ROBOTS\n")
    PROMPT()
    
    if(scanf("%d", &numb) < 1){
        ERROR("Invalid input\n")
        return false;
    }
    
    if(numb < 1){
        ERROR("Invalid number of robots\n")
        return false;
    }
    
    env->numbRobs = numb;
    
    // TODO
    // robsInit(robSet)
    
    emptyStdIn();
    DIALOG("\n")
    
    return true;
}

bool readNumbObsts(terra_t* env){
    int numb;
    
    DIALOG("ENTER NUMBER OF OBSTACLES\n")
    PROMPT()
    
    if(scanf("%d", &numb) < 1){
        ERROR("Invalid input\n")
        return false;
    }
    
    if(numb < 0){
        ERROR("Invalid number of obstacles\n")
        return false;
    }
    
    env->numbObsts = numb;
    
    emptyStdIn();
    DIALOG("\n")
    
    return true;
}

bool readObsts(terra_t* env){
    obst_t obst;
    unsigned int o;
    
    if(!env->numbObsts){
        return true;
    }
    
    DIALOG("ENTER POSITIONS OF OBSTACLES\n")
    for(o = 0; o < env->numbObsts; o++){
        if(!readCoord(&obst.top)){
            return false;
        }
           
        if(!readCoord(&obst.left)){
            return false;
        }
        
        if(!obstInMap(&obst, &env->map)){
            ERROR("Obstacle not in map\n")
            return false;
        }
        
        if(obstOnBorder(&obst, &env->map)){
            ERROR("Obstacle on border\n")
            return false;
        }
        
        if(obstCollides(&obst, &env->map)){
            ERROR("Obstacles too close\n")
            return false;
        }
        
        mapAddObstacle(&env->map, &obst);
    }
    
    DIALOG("\n")
    
    return true;
}

bool readCoord(unsigned int* coord){
    int start;
    int end;
    
    if(scanf("%d", &start) < 1){
        ERROR("Invalid input\n")
        return false;
    }
    
    if(getchar() == '-'){
        // range notation
        if(scanf("%d", &end) < 1){
            ERROR("Invalid input\n")
            return false;
        }
    }else{
        // isolated notation
        end = start;
    }
    
    if(start < 0 || end < 0){
        ERROR("Negative coordinate\n")
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

void loop(void){
    // TODO
}

void clean(terra_t* env){
    mapFree(&env->map);
}

int main(void){
    terra_t env;
    
    init(&env);
    loop();
    clean(&env);
    
    return EXIT_SUCCESS;
}