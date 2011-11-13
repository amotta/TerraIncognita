#ifndef TerraIncognita_main_h
#define TerraIncognita_main_h

#include <stdbool.h>

#include "types.h"

void emptyStdIn(void);
void printPrompt(void);
bool readDialogMode(terra_t*);
bool readDisplayMode(terra_t*);
bool readMapSize(terra_t*);
bool readAccessPoint(terra_t*);
bool readNumbRobs(terra_t*);
bool readNumbObsts(terra_t*);
bool readObsts(terra_t*);
bool readCoord(unsigned int*);
bool readData(terra_t*);
void init(terra_t*);
void loop(void);
void clean(terra_t*);
int main(void);

#endif