
#ifndef __SCREENS__
#define __SCREENS__

#include "utils.h"
#include "lib-game-states.h"


/* ============================== header functions ========================== */

// inicializa a tela de game over 
void initScreenGameOver();

// desenha a tela de game over
void drawScreenGameOver(char *str);

// desabilita a tela de game over
void deinitScreenGameOver(char *str);

// configuracoes da tela de ajuda
void helpScreen();

// configuracoes da tela de creditos
void creditScreen();

#endif 