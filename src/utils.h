
#ifndef __UTILS__
#define __UTILS__

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

// usado para definir qual o estado da maquina de estados do jogo
typedef enum STATE_GAME
{
	START, 
	SERVING, 
	PLAYING, 
	GAMEOVER, 
	ENDGAME,
	STATE_GAME_N
} STATE_GAME;

extern STATE_GAME state;

// usado para definir qual sprite ser usado de acordo com a direcao
typedef enum DIRECTIONS
{
	UP, 
	DOWN, 
	LEFT, 
	RIGHT,
	DIRECTIONS_N
} DIRECTIONS;

extern long frames;
extern long score;

void mustInit(bool test, const char *description);
int between(int lower, int higher);
float betweenF(float lower, float higher);
bool collide(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);

#endif