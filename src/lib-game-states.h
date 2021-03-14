
#ifndef __LIB_GAME_STATES__
#define __LIB_GAME_STATES__

#include "utils.h"
#include "enemy.h"
#include "player.h"
#include "screens.h"

/* ================================= extern's =============================== */

extern ALLEGRO_FONT            	*font;
extern STATE_GAME               state;
extern long                     score;
extern long                     frames;
extern ALLEGRO_TIMER           	*timer;
extern ALLEGRO_EVENT_QUEUE     	*queue;
extern VET_BLOCK_T	            *walls;
extern ALLEGRO_BITMAP			*buffer;
extern ALLEGRO_DISPLAY         	*display;
extern SPRITES_T                *sprites;
extern unsigned char            key[ALLEGRO_KEY_MAX];


/* ============================== header functions ========================== */

// faz a inicializacao do display
void createDisplay();

// desabilita o display
void destroyDisplay();

// configuracoes do display antes do desenho.
// torna o buffer alvo dos desenhos
void displayPreDraw();

// configuracoes do display apos o desenho ser feito no buffer.
// torna o display alvo e faz a escala do desenho feito no buffer para o display
void displayPostDraw();

// inicializa teclado para receber dados
void initKeyboard();

// atualiza dados vindo do teclado
void updateKeyboard(ALLEGRO_EVENT *event);

// inicializa os sprites do jogo
void initSprites();

// pega os sprites de um spritesheet e os coloca em variaveis
ALLEGRO_BITMAP *grabSprite(ALLEGRO_BITMAP *sheet, int x, int y, int w, int h);

// desabilita os sprites do jogo
void deinitSprites();

// inicializa o timer do jogo
void createTimer();

// desabilita o timer do jogo
void destroyTimer();

// inicializa a fila de eventos
void createQueue();

// desabilita a fila de eventos
void destroyQueue();

// inicializa os audios do jogo 
void initAudio();

// desabilita audios do jogo
void deinitAudio();

// inicializa variaveis de controle dos tiros do jogo
void initShots();

// adiciona os tiros em um vetor caso haja disparo
bool addShots(bool player, int x, int y, DIRECTIONS direction);

// atualiza os tiros que estao em tela
void updateShots();

// confere a colisao dos tiros com os elementos do jogo
bool collideShots(bool player, bool list, int x, int y, int w, int h);

// desenha na tela os tiros
void drawShots();

// desabilita os tiros do jogo
void deinitShots();

// inicializa o hud do jogo
void initHud();

// desenha na tela o hud
void drawHud();

// desabilita o hud
void deinitHud();

// inicializa o mapa do jogo com base em um arquivo "map.dat"
void initMap();

// cria as barreiras do jogo com base no arquivo lido
void createWalls(MAP_T *map);

// adiciona os blocks de forma individual para facilitar a destruicao por shots
void addIndividualBlocks(LIST_T *list, int x, int y);

// atualiza as barriras
void updateWalls();

// desenha as barreiras do jogo na tela
void drawMap();

// faz o controle das colisoes com blocos de parede
bool collideControl(int x, int y, int w, int h, DIRECTIONS direction);

// confere a colisao com as paredes
bool collideWalls(LIST_T *list, int x, int y, int w, int h, DIRECTIONS direction);

// confere a colisao com a agua
bool collideWater(int x, int y, int w, int h);

// desenha as arvores do jogo
void drawTrees();

// desabilita o mapa
void deinitMap();

// checa se o easter egg foi descoberto
bool checkIfEasterEgg(char *str);

// desenha easter egg do menu
void drawEasterEgg();

// inicializa os "efeitos especiais"
void initFX();

// adiciona explosao em um vetor caso haja destruicao de inimigo
void addFX(bool spark, int x, int y);

// atualiza o andamento da explosao
void updateFX();

// desenha a explosao
void drawFX();

// desabilita as explosoes
void deinitFX(); 

#endif