
#include "lib-game-states.h"

// variaveis globais externalizadas
ALLEGRO_TIMER           *timer;
ALLEGRO_EVENT_QUEUE     *queue;
ALLEGRO_DISPLAY         *display;
ALLEGRO_BITMAP			*buffer;
ALLEGRO_FONT            *font;
SPRITES                 sprites;
unsigned char key[ALLEGRO_KEY_MAX];

// variaveis globais



/* ================================ display ==================================*/
void createDisplay()
{
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    display = al_create_display(DISPLAY_W, DISPLAY_H);
    mustInit(display, "display");

    buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    mustInit(buffer, "bitmap buffer");
}

void destroyDisplay()
{
    al_destroy_bitmap(buffer);
    al_destroy_display(display);
}

void displayPreDraw()
{
    al_set_target_bitmap(buffer);
}

void displayPostDraw()
{
    al_set_target_backbuffer(display);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISPLAY_W, 
    DISPLAY_H, 0);

    al_flip_display();
}


/* ================================ keybord ==================================*/
void initKeyboard()
{
    memset(key, 0, sizeof(key));
}

void updateKeyboard(ALLEGRO_EVENT *event)
{
    switch(event->type)
    {
        case ALLEGRO_EVENT_TIMER:
            for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                key[i] &= KEY_SEEN;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            key[event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event->keyboard.keycode] &= KEY_RELEASED;
            break;
    }
}


/* ================================ sprites ==================================*/
ALLEGRO_BITMAP *grabSprite(int x, int y, int w, int h)
{
    ALLEGRO_BITMAP *sprite = al_create_sub_bitmap(sprites._sheet, x, y, w, h);
    mustInit(sprite, "sprite grab");
    return sprite;
}

void initSprites()
{
    sprites._sheet = al_load_bitmap("./resources/spritesheet.png");
    mustInit(sprites._sheet, "spritesheet");

    sprites.tank[UP]        = grabSprite(  0, 128, TANK_W, TANK_H);
    sprites.tank[DOWN]      = grabSprite( 64, 128, TANK_W, TANK_H);
    sprites.tank[LEFT]      = grabSprite( 32, 128, TANK_W, TANK_H);
    sprites.tank[RIGHT]     = grabSprite( 96, 128, TANK_W, TANK_H);

    sprites.life            = grabSprite(  0, 128, TANK_W, TANK_H);

    sprites.shot[UP]        = grabSprite(323, 102, SHOT_W, SHOT_H);
    sprites.shot[DOWN]      = grabSprite(339, 102, SHOT_W, SHOT_H);
    sprites.shot[LEFT]      = grabSprite(330, 102, SHOT_H, SHOT_W);
    sprites.shot[RIGHT]     = grabSprite(346, 102, SHOT_H, SHOT_W);

    sprites.enemies[UP]     = grabSprite(128, 144, TANK_W, TANK_H);
    sprites.enemies[DOWN]   = grabSprite(192, 144, TANK_W, TANK_H);
    sprites.enemies[LEFT]   = grabSprite(160, 144, TANK_W, TANK_H);
    sprites.enemies[RIGHT]  = grabSprite(224, 144, TANK_W, TANK_H);
}

void deinitSprites()
{
    al_destroy_bitmap(sprites.tank[RIGHT]);
    al_destroy_bitmap(sprites.tank[LEFT]);
    al_destroy_bitmap(sprites.tank[DOWN]);
    al_destroy_bitmap(sprites.tank[UP]);

    al_destroy_bitmap(sprites.life);

    al_destroy_bitmap(sprites.shot[RIGHT]);
    al_destroy_bitmap(sprites.shot[LEFT]);
    al_destroy_bitmap(sprites.shot[DOWN]);
    al_destroy_bitmap(sprites.shot[UP]);

    al_destroy_bitmap(sprites.enemies[RIGHT]);
    al_destroy_bitmap(sprites.enemies[LEFT]);
    al_destroy_bitmap(sprites.enemies[DOWN]);
    al_destroy_bitmap(sprites.enemies[UP]);

    al_destroy_bitmap(sprites._sheet);
}


/* ================================= timer ===================================*/
void createTimer()
{
	timer = al_create_timer(1.0 / 60.0);
    mustInit(timer, "timer");
}

void destroyTimer()
{
    al_destroy_timer(timer);
}


/* ================================= queue ===================================*/
void createQueue()
{
	queue = al_create_event_queue();
    mustInit(queue, "queue");
}

void destroyQueue()
{
    al_destroy_event_queue(queue);
}


/* ================================= font ====================================*/
void initFont()
{
	font = al_create_builtin_font();
    mustInit(font, "font");
}

void deinitFont()
{
    al_destroy_font(font);
}


/* ================================= audio ===================================*/
void initAudio()
{
	mustInit(al_install_audio(), "audio");
    mustInit(al_init_acodec_addon(), "audio codecs");
    mustInit(al_reserve_samples(16), "reserve samples");
}

void createAudio(ALLEGRO_SAMPLE *audio, char *name)
{
	audio = al_load_sample(name);
    mustInit(audio, "elephant");
}


/* ================================= shots ===================================*/

