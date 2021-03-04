
#ifndef __LIB_GAME_STATES__
#define __LIB_GAME_STATES__

#include "player.h"
#include "enemy.h"

// display stuffs
#define BUFFER_W 320
#define BUFFER_H 240
#define DISPLAY_SCALE 3
#define DISPLAY_W (BUFFER_W * DISPLAY_SCALE)
#define DISPLAY_H (BUFFER_H * DISPLAY_SCALE)
#define SHOT_W 3
#define SHOT_H 4

// keybord stuffs
#define KEY_SEEN     1
#define KEY_RELEASED 2

// sprites stuffs
typedef struct SPRITES
{
    ALLEGRO_BITMAP* _sheet;
    ALLEGRO_BITMAP* tank[4];
    ALLEGRO_BITMAP* shot[4];
    ALLEGRO_BITMAP* life;
    ALLEGRO_BITMAP* enemies[4];
} SPRITES;

// variaveis globais externalizadas 
extern ALLEGRO_TIMER           	*timer;
extern ALLEGRO_EVENT_QUEUE     	*queue;
extern ALLEGRO_DISPLAY         	*display;
extern ALLEGRO_BITMAP			*buffer;
extern ALLEGRO_FONT            	*font;
extern SPRITES                  sprites;
extern unsigned char key[ALLEGRO_KEY_MAX];


/* ============================== header functions ========================== */

// display stuffs
void createDisplay();
void destroyDisplay();
void displayPreDraw();
void displayPostDraw();

// keybord stuffs
void initKeyboard();
void updateKeyboard(ALLEGRO_EVENT *event);

// sprites stuffs
ALLEGRO_BITMAP *grabSprite(int x, int y, int w, int h);
void initSprites();
void deinitSprites();

// timer stuffs
void createTimer();
void destroyTimer();

// queue stuffs
void createQueue();
void destroyQueue();

// font stuffs
void initFont();
void deinitFont();

// audio stuffs
void initAudio();
void createAudio(ALLEGRO_SAMPLE *audio, char *name);


#endif