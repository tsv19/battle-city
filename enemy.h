
#ifndef __ENEMY__
#define __ENEMY__

#include "utils.h"
#include "lib-game-states.h"

#define ENEMY_N       20
#define TIME_MOVEMENT 80

typedef struct ENEMY_T
{
    int x, y;
    int shotTimer;
    bool used;
	DIRECTIONS direction;
} ENEMY_T;


// inicializa os inimigos do jogo
void initEnemies();

// atualiza o estado dos inimigos
void updateEnemies();

// realiza a movimentacao do inimigo
bool nextMove(ENEMY_T *enemies);

// desenha os inimigos na tela
void drawEnemies();

// desabilita os inimigos
void deinitEnemies();

#endif