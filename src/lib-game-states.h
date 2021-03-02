
#ifndef __LIB_GAME_STATES__
#define __LIB_GAME_STATES__

#include "utils.h"

// keybord stuffs
#define KEY_SEEN     1
#define KEY_RELEASED 2
extern unsigned char key[ALLEGRO_KEY_MAX];

#define BUFFER_W 320
#define BUFFER_H 240
#define DISPLAY_SCALE 3
#define DISPLAY_W (BUFFER_W * DISPLAY_SCALE)
#define DISPLAY_H (BUFFER_H * DISPLAY_SCALE)


extern ALLEGRO_TIMER           	*timer;
extern ALLEGRO_EVENT_QUEUE     	*queue;
extern ALLEGRO_DISPLAY         	*display;
extern ALLEGRO_BITMAP			*buffer;
extern ALLEGRO_FONT            	*font;


/* ============================== header functions ========================== */

// display stuffs
void createDisplay();
void destroyDisplay();
void displayPreDraw();
void displayPostDraw();

// keybord stuffs
void keyboardInit();
void keyboardUpdate(ALLEGRO_EVENT* event);


void createTimer();
void createQueue();


void createFont();


void initAudio();
void createAudio(ALLEGRO_SAMPLE* audio, char *name);




#endif