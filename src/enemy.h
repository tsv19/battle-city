
#ifndef __ENEMY__
#define __ENEMY__

#include "lib-game-states.h"
#include "utils.h"

#define ENEMY_N 20
#define TIME_MOVEMENT 100

typedef struct ENEMY_T
{
    int x, y;
	DIRECTIONS direction;
    int shotTimer;
    bool alive;
    bool used;
} ENEMY_T;

extern ENEMY_T *enemies;

// 
void initEnemies();

// 
void updateEnemies();

// 
void drawEnemies();

// 
void deinitEnemies();


#endif