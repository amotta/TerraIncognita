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
        printf("ERROR: Invalid input\n");
        return false;
    }
    
    if(in){
        env->dialogMode = false;
    }else{
        env->dialogMode = true;
    }
    
    if(env->dialogMode){
        printf("\n");
    }
    
    emptyStdIn();
    
    return true;
}

bool readDisplayMode(terra_t* env){
    if(env->dialogMode){
        printf(
            "SELECT RESULT MODE\n"
            "0\tComplete\n"
            "1\tRedirection\n"
        );
        printPrompt();
    }
    
    int in = 0;
    if(scanf("%d", &in) < 1){
        printf("ERROR: Invalid input\n");
        return false;
    }
    
    if(in){
        env->resultMode = false;
        
        if(env->dialogMode){
            printf(
                "< Redirection mode selected"
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
    int rows = 0;
    int cols = 0; 
    
    if(env->dialogMode){
        printf("ENTER MAP SIZE\n");
        printPrompt();
    }
    
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
    int rows = 0;
    int cols = 0;
    
    if(env->dialogMode){
        printf("ENTER POSITION OF ACCESS POINT\n");
        printPrompt();
    }
    
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
        printf("ERROR: Invalid input\n");
        return false;
    }
    
    if(numb < 0){
        printf("ERROR: Invalid number of obstacles\n");
        return false;
    }
    
    env->numbObsts = numb;
    
    if(env->dialogMode){
        printf(
            "< Number of obstacles: %u\n"
            "\n",
            env->numbObsts
        );
    }
    
    emptyStdIn();
    
    return true;
}

bool readObsts(terra_t* env){
    obst_t obst;
    unsigned int o;
    
    if(!env->numbObsts){
        return true;
    }
    
    if(env->dialogMode){
        printf("ENTER POSITIONS OF OBSTACLES\n");
        printPrompt();
    }
    
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
        
        if(env->dialogMode){
            printf(
                "< Position of obstacle %u: %u-%u %u-%u\n"
                "\n",
                o,
                obst.top,
                obst.bottom,
                obst.left,
                obst.right
            );
        }
    }
    
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