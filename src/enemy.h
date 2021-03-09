
#ifndef __ENEMY__
#define __ENEMY__

#include "utils.h"
#include "lib-game-states.h"

#define ENEMY_N       20
#define TIME_MOVEMENT 80

typedef struct ENEMY_T
{
    int x, y;
    bool used;
    bool alive;
	DIRECTIONS direction;
} ENEMY_T;

extern ENEMY_T *enemies;

// inicializa os inimigos do jogo
void initEnemies();

// atualiza o estado dos inimigos
void updateEnemies();

// desenha os inimigos na tela
void drawEnemies();

// desabilita os inimigos
void deinitEnemies();


#endif