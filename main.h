#include <stdbool.h>

#ifndef TerraIncognita_main_h
#define TerraIncognita_main_h

#define MODE_CONSOLE 0
#define MODE_REDIRECT 1

void emptyStdIn();
void printPrompt();
bool readDialogMode();
bool readDisplayMode();
bool readMapSize();
bool readAccessPoint();
bool readNumbRobs();
bool readNumbObs();
bool readObs();
bool readCoord(unsigned int*);
bool readData();
void init();
void loop();
void clean();
int main();

#endif