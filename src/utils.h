
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

typedef enum
{
	START, 
	SERVING, 
	PLAYING, 
	GAMEOVER, 
	ENDGAME
} STATE_GAME;

extern STATE_GAME state;

void mustInit(bool test, const char *description);

#endif