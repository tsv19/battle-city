
#ifndef __PLAYER__
#define __PLAYER__

#include "lib-game-states.h"
#include "utils.h"

#define TANK_SPEED 1
#define TANK_W 16
#define TANK_H 16
#define TANK_MAX_X (BUFFER_W - TANK_W)
#define TANK_MAX_Y (BUFFER_H - TANK_H)
#define MAX_LIVES 3
#define RESPAWN_TIMER 90
#define INVINCIBLE_TIMER 180

typedef struct PLAYER_T 
{
	int x, y;
    DIRECTIONS direction;
    int shotTimer;
    int lives;
    int respawnTimer;
    int invincibleTimer;
} PLAYER_T;


void initTank();

void updateTank();

void drawTank();

#endif