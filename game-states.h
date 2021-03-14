
#ifndef __GAME_STATES__
#define __GAME_STATES__

#include "lib-game-states.h"

// estado de inicializacao. realiza as configuracoes iniciais e exibe a 
// tela inicial para o jogador
void stateInit();

// estado de servico. realiza a preparacao dos elementos principais para 
// haver um jogo 
void stateServe();

// estado de play. realiza o controle do jogo, onde o jogo eh executado
void statePlay();

// estado de game over. apos o jogo ser perdido, exibe uma tela onde havera o
// score do jogador, asmaiores pontuacoes e a possibilidade de adicionar o nome
// para salvar na pontuacao
void stateGameOver();

// estado de fechamento do jogo. desabilita as configuracoes e encerra o jogo
void stateCloseGame();

#endif 