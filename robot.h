#ifndef TerraIncognita_robot_h
#define TerraIncognita_robot_h

#include "types.h"

void robInit(rob_t* rob);
void robSpawn(rob_t*, terra_t* env);
void robSetInit(robSet_t* set);
void robSetFree(robSet_t* set);

#endif