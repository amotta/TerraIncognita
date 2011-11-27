#ifndef TerraIncognita_robot_h
#define TerraIncognita_robot_h

#include "types.h"

void robInit(rob_t* rob);
void robSpawn(rob_t* rob, terra_t* env);
void robMovePrepare(rob_t* rob, terra_t* env);
void robMoveExplore(rob_t* rob, terra_t* env);
void robMoveAvoid(rob_t* rob, terra_t* env);
void robMove(rob_t* rob, terra_t* env);
void robSetInit(robSet_t* set);
void robSetFree(robSet_t* set);

#endif