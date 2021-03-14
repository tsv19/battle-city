
#ifndef __UTILS__
#define __UTILS__

#include <stdio.h>
#include <stdlib.h>
#include "tad-lde.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>


/* =============================== define's ================================= */

#define DISPLAY_SCALE 3
#define BUFFER_W 	  320
#define BUFFER_H 	  240
#define GAME_LIMIT_H (BUFFER_H) 
#define GAME_LIMIT_W (BUFFER_W - 48) 
#define DISPLAY_W	 (BUFFER_W * DISPLAY_SCALE)
#define DISPLAY_H 	 (BUFFER_H * DISPLAY_SCALE)

#define KEY_SEEN     1
#define KEY_RELEASED 2

#define SHOT_W     3
#define SHOT_H     4
#define SHOT_SPEED 3
#define SHOT_TIMER 50
#define SHOTS_N    128
#define BANANA_W   6
#define BANANA_H   6

#define SIZE_BLOCKS 8

#define SPARKS_FRAMES    3
#define EXPLOSION_FRAMES 2
#define FX_N             128

#define BASE_INIT_W 128
#define BASE_INIT_H 208

/* ================================ typedef's =============================== */

// usado para definir qual sprite de bloco do mapa vai ser usado
typedef enum BLOCK
{
    BRICK = 0,
    STEEL,
    WATER,
    TREE,
    CONTOUR,
    BLOCK_N
} BLOCK;

// usado para definir qual sprite ser usado de acordo com a direcao e para 
// definir/atualizar novas posicoes
typedef enum DIRECTIONS
{
	UP = 0, 
	DOWN, 
	LEFT, 
	RIGHT,
	DIRECTIONS_N
} DIRECTIONS;

// definido para indicar qual o estado da maquina de estados do jogo
typedef enum STATE_GAME
{
	START = 0, 
	SERVING, 
	PLAYING, 
	GAMEOVER, 
	ENDGAME,
	STATE_GAME_N
} STATE_GAME;

// definido para armazenar os sprites do jogo
typedef struct SPRITES_T
{
    ALLEGRO_BITMAP* DK;
    ALLEGRO_BITMAP* life;
    ALLEGRO_BITMAP* sheet;
    ALLEGRO_BITMAP* banana;
    ALLEGRO_BITMAP* insignia;
    ALLEGRO_BITMAP* block[BLOCK_N];
    ALLEGRO_BITMAP* sheetEasterEgg1;
    ALLEGRO_BITMAP* sheetEasterEgg2;
    ALLEGRO_BITMAP* shot[DIRECTIONS_N];
    ALLEGRO_BITMAP* tank[DIRECTIONS_N];
    ALLEGRO_BITMAP* sparks[SPARKS_FRAMES];
    ALLEGRO_BITMAP* enemies[DIRECTIONS_N];
    ALLEGRO_BITMAP* bananaShots[DIRECTIONS_N];
    ALLEGRO_BITMAP* explosion[EXPLOSION_FRAMES];
} SPRITES_T;

// definido para fazer controle dos tiros
typedef struct SHOT_T
{
    int x, y;
    int frame;
    bool used;
    bool player;
    DIRECTIONS direction;
} SHOT_T;

// definido para armazenar modelo de mapa
typedef struct MAP_T
{
    int **matrix;
    int line, column;
} MAP_T;

// definido para armazenar os high scores e ordena-los
typedef struct VET_SCORE_T
{
    long score;
    char name[10];
} VET_SCORE_T;

// definido para armazenar informacoes sobre os blocos
typedef struct VET_BLOCK_T
{
    LIST_T *bricks;
    LIST_T *steel;
    LIST_T *water;
    LIST_T *tree;
    LIST_T *contour;
} VET_BLOCK_T;

// definido para armazenar informacoes sobre explosoes
typedef struct FX
{
    int x, y;
    int frame;
    bool used;
    bool spark;
} FX;

/* ============================== header functions ========================== */

// faz a checagem da inicializacao 
void mustInit(bool test, const char *description);

// gera um numero aleatorio (inteiro) entre um intervalo
int between(int lower, int higher);

// gera um numero aleatorio (float) entre um intervalo
float betweenF(float lower, float higher);

// checa colisao entre dois alvos
bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);

#endif