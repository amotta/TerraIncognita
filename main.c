#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "main.h"

int dialogMode;
int displayMode;

// TODO
// Introduce structure for map
int mapRows;
int mapCols;

// TODO
// Same here. Add structure for map
int accessRow;
int accessCol;

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
    
    int rows = 0;
    int cols = 0;
    scanf("%d %d", &rows, &cols);
    
    emptyStdIn();
    printf("\n");
    
    if(rows > 0 && cols > 0){
        mapRows = rows;
        mapCols = cols;
        return true;
    }else{
        printf("ERROR: Map size is invalid\n");
        return false;
    }
}

bool readAccessField(){
    printf("ENTER POSITION OF ACCESS FIELD\n");
    printPrompt();
    
    int row = 0;
    int col = 0;
    scanf("%d %d", &row, &col);
    
    emptyStdIn();
    printf("\n");
    
    bool inMap = false;
    bool onBorder = false;
    
    // check if position is in map
    if(0 <= row && row < mapRows && 0 <= col && col < mapCols){
        inMap = true;
    }
    
    // check if position is on border
    if(row == 0 || row == mapRows - 1 || col == 0 || col == mapCols - 1){
        onBorder = true;
    }
    
    if(inMap && onBorder){
        return true;
    }else{
        printf("ERROR: Position of access field is invalid\n");
        return false;
    }
}

bool readData(){
    bool valid = true;
    
    readDialogMode();
    readDisplayMode();
    
    valid = valid && readMapSize();
    if(valid){
        // TODO
        // initMap
    }
    
    valid = valid && readAccessField();
    
    return true;
}

bool init(){
    readData();
    return true;
}

bool loop(){
    return true;
}

int main(){
    init();
    loop();
    
    return EXIT_SUCCESS;
}