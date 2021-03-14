
#ifndef __PLAYER__
#define __PLAYER__

#include "utils.h"
#include "lib-game-states.h"

/* =============================== define's ================================= */

#define TANK_SPEED       1
#define MAX_LIVES        3
#define TANK_W           16
#define TANK_H           16
#define RESPAWN_TIMER    10
#define INVINCIBLE_TIMER 50
#define TANK_INIT_W      128
#define TANK_INIT_H      176
#define TANK_MAX_Y       (GAME_LIMIT_H - TANK_H)
#define TANK_MAX_X       (GAME_LIMIT_W - TANK_W)

/* ================================ typedef's =============================== */

// define os dados de um jogador 
typedef struct PLAYER_T 
{
	int x, y;
    int lives;
    int shotTimer;
    int respawnTimer;
    int invincibleTimer;
    DIRECTIONS direction;
} PLAYER_T;


/* ============================== header functions ========================== */

// inicializa tank do player
void initTank();

// atualiza o tank do player
void updateTank();

// desenha o tank do player
void drawTank();

// desabilita tank do player
void deinitTank();

#endif