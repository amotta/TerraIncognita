#include <stdbool.h>

#ifndef TerraIncognita_main_h
#define TerraIncognita_main_h

#define BUF_LEN 1024

#define MODE_CONSOLE 0
#define MODE_REDIRECT 1

void emptyStdIn();
void printPrompt();
void readDialogMode();
void readDisplayMode();
bool readMapSize();
bool readAccessPoint();
bool readNumbRobs();
void readNumbObs();
bool readObs();
bool readData();
bool init();
bool loop();
bool clean();
int main();

#endif