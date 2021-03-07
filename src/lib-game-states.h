
#ifndef __LIB_GAME_STATES__
#define __LIB_GAME_STATES__

#include "player.h"
#include "enemy.h"


/* =============================== define's ================================= */

// display stuffs
#define BUFFER_W 320
#define BUFFER_H 240
#define DISPLAY_SCALE 3
#define DISPLAY_W (BUFFER_W * DISPLAY_SCALE)
#define DISPLAY_H (BUFFER_H * DISPLAY_SCALE)
#define GAME_LIMIT_W (BUFFER_W - 48) 
#define GAME_LIMIT_H (BUFFER_H) 

// keybord stuffs
#define KEY_SEEN     1
#define KEY_RELEASED 2

// sprites stuffs
#define BLOCKS 8

// shot stuffs
#define SHOTS_N 128
#define SHOT_W 3
#define SHOT_H 4
#define SHOT_SPEED 3
#define SHOT_TIMER 50


/* ================================ typedef's =============================== */

typedef struct SPRITES_T
{
    ALLEGRO_BITMAP* sheet;
    ALLEGRO_BITMAP* tank[DIRECTIONS_N];
    ALLEGRO_BITMAP* shot[DIRECTIONS_N];
    ALLEGRO_BITMAP* enemies[DIRECTIONS_N];
    ALLEGRO_BITMAP* life;
    ALLEGRO_BITMAP* brickWall;
    ALLEGRO_BITMAP* steelWall;
    ALLEGRO_BITMAP* water;
    ALLEGRO_BITMAP* tree;
    ALLEGRO_BITMAP* contour;
} SPRITES_T;

typedef struct SHOT_T
{
    int x, y;
    DIRECTIONS direction;
    int frame;
    bool player;
    bool used;
} SHOT_T;

// map stuffs
typedef struct MAP_T
{
    int line, column;
    int **matrix;
} MAP_T;

typedef struct VET_SCORE_T
{
    char name[10];
    long score;
} VET_SCORE_T;


/* ================================= extern's =============================== */

// variaveis globais externalizadas 
extern ALLEGRO_TIMER           	*timer;
extern ALLEGRO_EVENT_QUEUE     	*queue;
extern ALLEGRO_DISPLAY         	*display;
extern ALLEGRO_BITMAP			*buffer;
extern ALLEGRO_FONT            	*font;
extern SPRITES_T                *sprites;
extern unsigned char            key[ALLEGRO_KEY_MAX];
extern SHOT_T                   *shots;
extern MAP_T                    *map;


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
void initSprites();
ALLEGRO_BITMAP *grabSprite(int x, int y, int w, int h);
void deinitSprites();

// timer stuffs
void createTimer();
void destroyTimer();

// queue stuffs
void createQueue();
void destroyQueue();

// audio stuffs
void initAudio();
void createAudio(ALLEGRO_SAMPLE *audio, char *name);
void deinitAudio();

// shots stuffs
void initShots();
bool addShots(bool player, bool straight, int x, int y, DIRECTIONS direction);
void updateShots();
bool collideShots(bool player, int x, int y, int w, int h);
void drawShots();
void deinitShots();

// hud
void initHud();
void deinitHud();
void updateHud();
void drawHud();

// hud game over 
VET_SCORE_T *initHudGameOver();
void drawHudGameOver(VET_SCORE_T *highScores, char *str);
void deinitHudGameOver(VET_SCORE_T *highScores, char *str);

//create walls stuffs
MAP_T * initMap();
void drawWalls(MAP_T *map);


#endif