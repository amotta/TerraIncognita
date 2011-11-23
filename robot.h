#ifndef TerraIncognita_robot_h
#define TerraIncognita_robot_h

#include "types.h"

void robInit(rob_t* rob);
void robSetInit(robSet_t* set);
void robSetPlan(robSet_t* set, map_t* map);
void robSetFree(robSet_t* set);

#endif