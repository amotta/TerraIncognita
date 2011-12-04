#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "main.h"
#include "map.h"
#include "obstacle.h"
#include "point.h"
#include "robot.h"

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
                env->robs.set[r].dist = (dim - 2 - 2 * r);
                break;
                
            case DIR_BOTTOM:
            case DIR_RIGHT:
                env->robs.set[r].dist = 1 + 2 * r;
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
    unsigned int r;
    bool done = false;
    rob_t* rob = NULL;
    
    env->numbCycles = 0;
    
    while(!done && env->numbCycles < 2 * env->map.length){
        done = true;
        
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
            done = false;
            robSpawn(env);
        }
        
        if(env->robs.active){
            done = false;
        }
        
        // try to complete map using rules
        mapComplete(&env->robs.map, &env->map);
        
        if(env->resultMode){
            mapPrint(&env->robs.map);
        }
        
        env->numbCycles++;
    }
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
    }
    
    clean(&env);
    
    return EXIT_SUCCESS;
}