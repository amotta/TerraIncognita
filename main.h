#ifndef TerraIncognita_main_h
#define TerraIncognita_main_h

#include "types.h"

void emptyStdIn(void);
void printPrompt(void);
bool readDialogMode(terra_t*);
bool readResultMode(terra_t*);
bool readMapSize(terra_t*);
bool readAccessPoint(terra_t*);
bool readNumbRobs(terra_t*);
bool readNumbObsts(terra_t*);
bool readObsts(terra_t*);
bool readCoord(unsigned int*);
bool readData(terra_t*);
void plan(terra_t*);
bool init(terra_t*);
void loop(terra_t*);
void stats(terra_t*);
void clean(terra_t*);
int main(void);

#endif