
#include "lib-game-states.h"

// variaveis globais externalizadas
ALLEGRO_TIMER           *timer;
ALLEGRO_EVENT_QUEUE     *queue;
ALLEGRO_DISPLAY         *display;
ALLEGRO_BITMAP			*buffer;
ALLEGRO_FONT            *font;
SPRITES                 sprites;
unsigned char key[ALLEGRO_KEY_MAX];
SHOT shots[SHOTS_N];

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
void initShots()
{
    for(int i = 0; i < SHOTS_N; i++)
        shots[i].used = false;
}

bool addShots(bool player, bool straight, int x, int y, DIRECTIONS direction)
{
    // al_play_sample(
    //     sample_shot,
    //     0.3,
    //     0,
    //     player ? 1.0 : between_f(1.5, 1.6),
    //     ALLEGRO_PLAYMODE_ONCE,
    //     NULL
    // );

    for(int i = 0; i < SHOTS_N; i++)
    {
        if(shots[i].used)
            continue;

        shots[i].player = player;

        if(player)
        {
            shots[i].direction = direction;

            if (shots[i].direction == UP)
            {
                shots[i].x = x - (SHOT_W / 2) - 1;
                shots[i].y = y - (TANK_H / 2);
            }
            if (shots[i].direction == DOWN)
            {
                shots[i].x = x - (SHOT_W / 2) - 1;
                shots[i].y = y + SHOT_W + 1;
            }
            if (shots[i].direction == LEFT)
            {
                shots[i].x = x - (TANK_W / 2);
                shots[i].y = y - 2;
            }
            if (shots[i].direction == RIGHT)
            {
                shots[i].x = x + SHOT_W + 1;
                shots[i].y = y - (SHOT_H / 2);
            }            
        }
        else // enemy
        {
            shots[i].x = x - (SHOT_W / 2);
            shots[i].y = y - (SHOT_H / 2);

            if(straight)
            {
                shots[i].dx = 0;
                shots[i].dy = 2;
            }
            else
            {
                shots[i].dx = between(-2, 2);
                shots[i].dy = between(-2, 2);
            }

            // if the shot has no speed, don't bother
            if(!shots[i].dx && !shots[i].dy)
                return true;

            shots[i].frame = 0;
        }

        shots[i].frame = 0;
        shots[i].used = true;

        return true;
    }
    return false;
}

void updateShots()
{
    for(int i = 0; i < SHOTS_N; i++)
    {
        if(!shots[i].used)
            continue;

        if(shots[i].player)
        {
            if (shots[i].direction == UP)
                shots[i].y -= SHOT_SPEED;
            if (shots[i].direction == DOWN)
                shots[i].y += SHOT_SPEED;
            if (shots[i].direction == LEFT)
                shots[i].x -= SHOT_SPEED;
            if (shots[i].direction == RIGHT)
                shots[i].x += SHOT_SPEED;


            if(shots[i].y < -SHOT_H)
            {
                shots[i].used = false;
                continue;
            }
        }
        else // alien
        {
            shots[i].x += shots[i].dx;
            shots[i].y += shots[i].dy;

            if((shots[i].x < -SHOT_W)
            || (shots[i].x > BUFFER_W)
            || (shots[i].y < -SHOT_H)
            || (shots[i].y > BUFFER_H)
            ) {
                shots[i].used = false;
                continue;
            }
        }

        shots[i].frame++;
    }
}

bool collideShots(bool player, int x, int y, int w, int h)
{
    for(int i = 0; i < SHOTS_N; i++)
    {
        if(!shots[i].used)
            continue;

        // don't collide with one's own shots
        if(shots[i].player == player)
            continue;

        int sw, sh;
        if(player)
        {
            sw = SHOT_W;
            sh = SHOT_H;
        }
        else
        {
            sw = SHOT_W;
            sh = SHOT_H;
        }

        if(collide(x, y, x+w, y+h, shots[i].x, shots[i].y, shots[i].x+sw, shots[i].y+sh))
        {
            // fx_add(true, shots[i].x + (sw / 2), shots[i].y + (sh / 2));
            shots[i].used = false;
            return true;
        }
    }

    return false;
}

void drawShots()
{
    for(int i = 0; i < SHOTS_N; i++)
    {
        if(!shots[i].used)
            continue;

        int frame_display = (shots[i].frame / 2) % 2;

        if(shots[i].player)
            al_draw_bitmap(sprites.shot[shots[i].direction], shots[i].x, 
            shots[i].y, 0);
        else // alien
        {
            ALLEGRO_COLOR tint =
                frame_display
                ? al_map_rgb_f(1, 1, 1)
                : al_map_rgb_f(0.5, 0.5, 0.5)
            ;
            
            al_draw_tinted_bitmap(sprites.shot[shots[i].direction], tint, 
            shots[i].x, shots[i].y, 0);
        }
    }
}