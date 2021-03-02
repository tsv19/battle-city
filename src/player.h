
#ifndef __PLAYER__
#define __PLAYER__

#include "utils.h"

typedef struct PLAYER_T 
{
	int x, y;
    int shotTimer;
    int lives;
    int respawnTimer;
    int invincibleTimer;
} PLAYER_T;

#endif