#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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
        printf(
            "< Dialog mode selected\n"
            "\n"
        );
    }
    
    emptyStdIn();
    
    return true;
}

bool readDisplayMode(terra_t* env){
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
        printf("ERROR: Invalid input\n");
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
        printf("ERROR: Invalid input\n");
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
        printf("ERROR: Invalid input\n");
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
            printf("ERROR: Obstacle not in map\n");
            return false;
        }
        
        if(obstOnBorder(&obst, &env->map)){
            printf("ERROR: Obstacle on border\n");
            return false;
        }
        
        if((coll = obstSetCheck(&env->obsts, &obst)) >= 0){
            printf("ERROR: Obstacles %u and %u are too close\n", coll, o);
            obstPrint(&env->obsts.set[coll]);
            obstPrint(&obst);
            return false;
        }
        
        obstSetAdd(&env->obsts, &obst);
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
    obstSetFree(&env->obsts);
}

int main(void){
    terra_t env;
    
    init(&env);
    loop();
    clean(&env);
    
    return EXIT_SUCCESS;
}