
#include "lib-game-states.h"

// lib-game-states
FX                      *fx;
MAP_T                   *map;
ALLEGRO_FONT            *font;
PLAYER_T                *tank;
ALLEGRO_TIMER           *timer;
SHOT_T                  *shots;
ALLEGRO_EVENT_QUEUE     *queue;
ALLEGRO_BITMAP			*buffer;
long                    **walls;
ALLEGRO_DISPLAY         *display;
SPRITES_T               *sprites;
long                    scoreHud;
bool                    easterEgg;
VET_SCORE_T             *highScores;
ALLEGRO_SAMPLE          *sampleShot;
ALLEGRO_SAMPLE          *sampleExplode;
unsigned char           key[ALLEGRO_KEY_MAX];

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

void initSprites()
{
    if (!sprites)
    {
        sprites = malloc(sizeof(SPRITES_T));
        mustInit(sprites, "sprites variable");
    }

    sprites->sheet = al_load_bitmap("./resources/spritesheet.png");
    mustInit(sprites->sheet, "spritesheet battle city");

    sprites->sheetEasterEgg1 = al_load_bitmap("./resources/spritesheet2.png");
    mustInit(sprites->sheetEasterEgg1, "spritesheet DK");

    sprites->sheetEasterEgg2 = al_load_bitmap("./resources/spritesheet3.png");
    mustInit(sprites->sheetEasterEgg2, "spritesheet banana");

    sprites->tank[UP]    = grabSprite(sprites->sheet,   0, 128, TANK_W, TANK_H);
    sprites->tank[DOWN]  = grabSprite(sprites->sheet,  64, 128, TANK_W, TANK_H);
    sprites->tank[LEFT]  = grabSprite(sprites->sheet,  32, 128, TANK_W, TANK_H);
    sprites->tank[RIGHT] = grabSprite(sprites->sheet,  96, 128, TANK_W, TANK_H);

    sprites->life = grabSprite(sprites->sheet,   0, 128, TANK_W, TANK_H);

    sprites->shot[UP]    = grabSprite(sprites->sheet, 323, 102, SHOT_W, SHOT_H);
    sprites->shot[DOWN]  = grabSprite(sprites->sheet, 339, 102, SHOT_W, SHOT_H);
    sprites->shot[LEFT]  = grabSprite(sprites->sheet, 330, 102, SHOT_H, SHOT_W);
    sprites->shot[RIGHT] = grabSprite(sprites->sheet, 346, 102, SHOT_H, SHOT_W);

    sprites->bananaShots[UP]    = grabSprite(sprites->sheet, 322,  85, BANANA_W, BANANA_H);
    sprites->bananaShots[DOWN]  = grabSprite(sprites->sheet, 338,  85, BANANA_W, BANANA_H);
    sprites->bananaShots[LEFT]  = grabSprite(sprites->sheet, 330,  85, BANANA_W, BANANA_W);
    sprites->bananaShots[RIGHT] = grabSprite(sprites->sheet, 346,  85, BANANA_W, BANANA_W);

    sprites->enemies[UP]    = grabSprite(sprites->sheet, 128, 144, TANK_W, TANK_H);
    sprites->enemies[DOWN]  = grabSprite(sprites->sheet, 192, 144, TANK_W, TANK_H);
    sprites->enemies[LEFT]  = grabSprite(sprites->sheet, 160, 144, TANK_W, TANK_H);
    sprites->enemies[RIGHT] = grabSprite(sprites->sheet, 224, 144, TANK_W, TANK_H);
 
    sprites->block[BRICK]   = grabSprite(sprites->sheet, 256,  64, SIZE_BLOCKS, SIZE_BLOCKS);
    sprites->block[STEEL]   = grabSprite(sprites->sheet, 256,  72, SIZE_BLOCKS, SIZE_BLOCKS);
    sprites->block[WATER]   = grabSprite(sprites->sheet, 264,  80, SIZE_BLOCKS, SIZE_BLOCKS);
    sprites->block[TREE]    = grabSprite(sprites->sheet, 264,  72, SIZE_BLOCKS, SIZE_BLOCKS);
    sprites->block[CONTOUR] = grabSprite(sprites->sheet, 272,  72, SIZE_BLOCKS, SIZE_BLOCKS);

    sprites->sparks[0] = grabSprite(sprites->sheet, 259, 130, 10, 11);
    sprites->sparks[1] = grabSprite(sprites->sheet, 272, 129, 15, 15);
    sprites->sparks[2] = grabSprite(sprites->sheet, 288, 128, 16, 16);

    sprites->explosion[0]    = grabSprite(sprites->sheet,   304, 129, 31, 30);
    sprites->explosion[1]    = grabSprite(sprites->sheet,   335, 128, 32, 32);

    sprites->DK     = grabSprite(sprites->sheetEasterEgg1,   0, 0, 63, 56);
    sprites->banana = grabSprite(sprites->sheetEasterEgg2,   0, 0, 30, 31);
}


ALLEGRO_BITMAP *grabSprite(ALLEGRO_BITMAP *sheet, int x, int y, int w, int h)
{
    ALLEGRO_BITMAP *sprite = al_create_sub_bitmap(sheet, x, y, w, h);
    mustInit(sprite, "sprite grab");
    return sprite;
}


void deinitSprites()
{
    for (int i = 0; i < DIRECTIONS_N; i++)
    {
        al_destroy_bitmap(sprites->tank[i]);
    }

    al_destroy_bitmap(sprites->life);

    for (int i = 0; i < DIRECTIONS_N; i++)
    {
        al_destroy_bitmap(sprites->shot[i]);
    }
    
    for (int i = 0; i < DIRECTIONS_N; i++)
    {
        al_destroy_bitmap(sprites->enemies[i]);
    }

    for (int i = 0; i < DIRECTIONS_N; i++)
    {
        al_destroy_bitmap(sprites->bananaShots[i]);
    }
    
    for (int i = 0; i < BLOCK_N; i++)
    {
        al_destroy_bitmap(sprites->block[i]);
    }

    for (int i = 0; i < SPARKS_FRAMES; i++)
    {
        al_destroy_bitmap(sprites->sparks[i]);
    }

    for (int i = 0; i < EXPLOSION_FRAMES; i++)
    {
        al_destroy_bitmap(sprites->explosion[i]);
    }

    al_destroy_bitmap(sprites->DK);
    al_destroy_bitmap(sprites->banana);

    al_destroy_bitmap(sprites->sheet);
    al_destroy_bitmap(sprites->sheetEasterEgg1);
    al_destroy_bitmap(sprites->sheetEasterEgg2);

    free(sprites);
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


/* ================================= audio ===================================*/

void initAudio()
{
	mustInit(al_install_audio(), "audio");
    mustInit(al_init_acodec_addon(), "audio codecs");
    mustInit(al_reserve_samples(128), "reserve samples");

    sampleShot = al_load_sample("./resources/shot.wav");
    mustInit(sampleShot, "shot sample");

    sampleExplode = al_load_sample("./resources/explode.wav");
    mustInit(sampleExplode, "explode sample");
    
}

void deinitAudio()
{
    al_destroy_sample(sampleShot);
    al_destroy_sample(sampleExplode);
} 


/* ================================= shots ===================================*/

void initShots()
{
    if (!shots)
    {
        shots = (SHOT_T *)calloc(SHOTS_N, sizeof(SHOT_T));
        mustInit(shots, "shots");
    }

    for(int i = 0; i < SHOTS_N; i++)
        shots[i].used = false;
}


bool addShots(bool player, bool straight, int x, int y, DIRECTIONS direction)
{
    al_play_sample(sampleShot, 0.1, 0, player ? 1.0 : betweenF(1.5, 1.6), 
                    ALLEGRO_PLAYMODE_ONCE, NULL);

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

        if (shots[i].direction == UP)
            shots[i].y -= SHOT_SPEED;
        if (shots[i].direction == DOWN)
            shots[i].y += SHOT_SPEED;
        if (shots[i].direction == LEFT)
            shots[i].x -= SHOT_SPEED;
        if (shots[i].direction == RIGHT)
            shots[i].x += SHOT_SPEED;

        if((shots[i].x < -SHOT_W)
            || (shots[i].x > GAME_LIMIT_W)
            || (shots[i].y < -SHOT_H)
            || (shots[i].y > GAME_LIMIT_H))
        {
            shots[i].used = false;
            continue;
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
        
        if (!easterEgg)
        {
            sw = SHOT_W;
            sh = SHOT_H;
        }
        {
            sw = BANANA_W;
            sh = BANANA_H;
        }

        if(collide(x, y, x+w, y+h, shots[i].x, shots[i].y, shots[i].x+sw, shots[i].y+sh))
        {
            addFX(true, shots[i].x + (sw / 2), shots[i].y + (sh / 2));

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
        {
            if (easterEgg)
            {
                al_draw_bitmap(sprites->bananaShots[shots[i].direction], shots[i].x, 
                            shots[i].y, 0);
            }
            else
            {
            al_draw_bitmap(sprites->shot[shots[i].direction], shots[i].x, 
                        shots[i].y, 0);
            }
        }    
        else // enemy
        {
            ALLEGRO_COLOR tint = frame_display 
                        ? al_map_rgb_f(1, 1, 1) : al_map_rgb_f(0.5, 0.5, 0.5);

            if (easterEgg)
            {
                al_draw_tinted_bitmap(sprites->bananaShots[shots[i].direction], tint, 
                                shots[i].x, shots[i].y, 0);
            }
            else
            {
                al_draw_tinted_bitmap(sprites->shot[shots[i].direction], tint, 
                                shots[i].x, shots[i].y, 0);
            }
        }
    }
}


void deinitShots(){
    free(shots);
}


/* ================================== hud  ===================================*/

void initHud()
{
    font = al_create_builtin_font();
    mustInit(font, "font");
    score = 0;
}


void deinitHud()
{
    al_destroy_font(font);
}


void updateHud()
{
    if(frames % 2)
        return;

    for(long i = 5; i > 0; i--)
    {
        long diff = 1 << i;
        if(scoreHud <= (score - diff))
            scoreHud += diff;
    }
}


void drawHud()
{
    al_draw_text(font, al_map_rgb_f(1,1,1), BUFFER_W - 50, 4, 0, "SCORE");
    al_draw_textf(font, al_map_rgb_f(1,1,1), BUFFER_W - 50, 14, 0, "%06ld", score);

    int spacing = TANK_W + 1;
    for(int i = 0; i < tank->lives; i++)
    {
        al_draw_bitmap(sprites->life, (BUFFER_W - 50) + (i * spacing), 24, 0);
    }

}

/* =========================== screen game over ==============================*/

void initScreenGameOver()
{
    FILE* scores = fopen("./resources/highscores.txt", "r");
    mustInit(scores, "file of high scores");
    
    highScores = (VET_SCORE_T *)malloc(11 * sizeof(VET_SCORE_T));    
    mustInit(highScores, "High Scores");
    for(int i = 0; i < 10; i++)
    { 
        fscanf(scores, "%s %li", highScores[i].name, &highScores[i].score);
    }

    fclose(scores);
}


void drawScreenGameOver(char *str)
{
    al_draw_textf(font, al_map_rgb(255,  100,   70), BUFFER_W/2, 3, 
                ALLEGRO_ALIGN_CENTER, "GAME OVER");

    al_draw_textf(font, al_map_rgb(255,  70,   0), BUFFER_W/1.5, 30, 
                ALLEGRO_ALIGN_CENTER, "HIGH SCORES:");
    al_draw_textf(font, al_map_rgb(255, 255, 255), BUFFER_W/4, 30, 
                ALLEGRO_ALIGN_CENTER, "YOUR SCORE:");

    al_draw_textf(font, al_map_rgb(255, 255, 255), BUFFER_W/4, 50, 
                ALLEGRO_ALIGN_CENTER, "Nome:");

    al_draw_text(font, al_map_rgb(255, 255, 255), BUFFER_W/4, 80, 
                ALLEGRO_ALIGN_CENTER, str);
    al_draw_textf(font, al_map_rgb(255, 255, 255), BUFFER_W/4, 110, 
                ALLEGRO_ALIGN_CENTER, "%li", score);

    for (int i = 0; i < 10 ; i++)
    {
        al_draw_text(font, al_map_rgb(255, 25*i, 25*i), BUFFER_W/1.5, 
                    50 + 15 * i, ALLEGRO_ALIGN_CENTER, highScores[i].name);

        al_draw_textf(font, al_map_rgb(255, 25*i, 25*i), BUFFER_W/1.2, 
                    50 + 15 * i , ALLEGRO_ALIGN_CENTER, "%li",highScores[i].score);
    }

    al_draw_textf(font, al_map_rgb(200, 200, 255), BUFFER_W/4, 170, 
                ALLEGRO_ALIGN_CENTER, "[ENTER] REINICIAR");
    al_draw_textf(font, al_map_rgb(255, 200, 200), BUFFER_W/4, 200, 
                ALLEGRO_ALIGN_CENTER, "[ESC] SAIR");
            
}


void deinitScreenGameOver(char *str)
{
    if (!strlen(str))
        strcpy(str, "player1");
    
    strcpy(highScores[10].name, str); 
    highScores[10].score = score;

    VET_SCORE_T aux;

    for (int i = 1; i < 11; i++) {
        for (int j = 0; j < 11 - i; j++) {
            if (highScores[j].score > highScores[j + 1].score) {
                aux = highScores[j];
                highScores[j] = highScores[j + 1];
                highScores[j + 1] = aux;
            }
        }
    }

    FILE* scores = fopen("./resources/highscores.txt", "w+");
    mustInit(scores, "file highscores.txt");

    for(int i = 10; i > 0; i--)
    {
        fprintf(scores,"%s %li\n", highScores[i].name, highScores[i].score);
    }

    fclose(scores);
    free(highScores);
}


/* ============================== create walls ==============================*/

void initMap()
{
    FILE  *mapGame = fopen("./resources/map.dat", "r");
    mustInit(mapGame, "file map.dat");
    
    map = (MAP_T *)malloc(sizeof(MAP_T));
    mustInit(map, "map variable");

    mustInit(fscanf(mapGame, "%d %d", &map->line, &map->column) == 2, "size of map");

    map->matrix = (int **)calloc(map->line, sizeof(int *));
    mustInit(map, "matrix map");

    for (int i = 0; i < map->line; i++) 
    {
        map->matrix[i] = (int *)calloc(map->column, sizeof(int));
        mustInit(map->matrix[i], "line of matrix of map");
    }
    
    for (int i = 0; i < map->line; i++) 
    {
        for (int j = 0; j < map->column; j++) 
        {
            mustInit(fscanf(mapGame, "%d", &map->matrix[i][j]) == 1, "map values");
        }
    }

    fclose(mapGame);
}


void createWalls()
{

     

    if (!walls)
    {
        walls = (int **)calloc(BLOCK_N, sizeof(BLOCK_T*));
    }
}


void drawWalls()
{
    for (int i = 0; i < map->line; i++)
    {
        for (int j = 0; j < map->column; j++)
        {
            if (map->matrix[i][j] == 1)
            {
                al_draw_bitmap(sprites->block[CONTOUR], j * 16, i * 16, 0);
            }
        }
    }
}


void deinitMap()
{
    free(map);
}

/* =============================== help screen ============================== */

void helpScreen()
{
    al_draw_bitmap(sprites->life, 10, 5, 0);
    al_draw_text(font, al_map_rgb(255, 255, 255), 30, 12, 0, 
                "<- This is you");
    
    al_draw_bitmap(sprites->enemies[UP], 10, 25, 0);
    al_draw_text(font, al_map_rgb(255, 255, 255), 30, 32, 0, 
                "<- This is your enemy");

    al_draw_bitmap(sprites->block[BRICK], 15, 52, 0);
    al_draw_text(font, al_map_rgb(255, 255, 255), 30, 52, 0, 
                "<- Can destroy if you shot it");

    al_draw_bitmap(sprites->block[STEEL], 15, 72, 0);
    al_draw_text(font, al_map_rgb(255, 255, 255), 30, 72, 0, 
                "<- Can't destroy if you shot it");

    al_draw_bitmap(sprites->block[WATER], 15, 92, 0);
    al_draw_text(font, al_map_rgb(255, 255, 255), 30, 92, 0, 
                "<- Water + tank = not cool");

    al_draw_bitmap(sprites->block[TREE], 15, 112, 0);
    al_draw_text(font, al_map_rgb(255, 255, 255), 30, 112, 0, 
                "<- Tree + tank = where am I?");

    al_draw_text(font, al_map_rgb(255, 255, 255), 10, 160, 0, 
                "* Use the arrow keys to move the tank");

    al_draw_text(font, al_map_rgb(255, 255, 255), 10, 180, 0, 
                "* Use the space bar to shot");

    al_draw_text(font, al_map_rgb(255, 255, 255), 10, 200, 0, 
                "* Try type \"kong\" in menu and play ;)");
    
    al_draw_text(font, al_map_rgb(255, 50, 80), BUFFER_W/2, 220, 
                ALLEGRO_ALIGN_CENTER, "* To close the help [H][F1]");
}

/* ============================= credit screen ============================== */

void creditScreen()
{
    al_draw_text(font, al_map_rgb(255, 255, 255), 5, 10, 0, 
                "* Author: Tiago Serique :)");

    al_draw_text(font, al_map_rgb(255, 255, 255), 5, 30, 0, 
                "* If you wanna see my face:");
    
    al_draw_text(font, al_map_rgb(255, 255, 255), 20, 50, 0, 
                "Instagram (@tecseit)");

    al_draw_text(font, al_map_rgb(255, 255, 255), 20, 70, 0, 
                "GitHub (@tiagoserique)");
    
    al_draw_text(font, al_map_rgb(255, 255, 255), 5, 90, 0, 
                "* Sounds:");
            
    al_draw_text(font, al_map_rgb(255, 255, 255), 20, 110, 0, 
                "www.freesounds.org");
    
    al_draw_text(font, al_map_rgb(255, 255, 255), 5, 130, 0, 
                "* Spritesheets:");

    al_draw_text(font, al_map_rgb(255, 255, 255), 20, 150, 0, 
                "www.spriters-resource.com");

    al_draw_text(font, al_map_rgb(255, 50, 80), BUFFER_W/2, 220, 
                ALLEGRO_ALIGN_CENTER, "* To close the credits [C]");
}


/* ============================== easter egg ================================ */

bool checkIfEasterEgg(char *str)
{
    if (strstr(str, "kong") != NULL)
    {
        return easterEgg = true;
    }

    return false;
}

void drawEasterEgg()
{
    al_draw_bitmap(sprites->banana, BUFFER_W/1.5 + 30, BUFFER_H/1.5, 0);
    al_draw_bitmap(    sprites->DK,              30,         20, 0);
}


/* ========================== FX, explosion, etc ============================ */

void initFX()
{
    if (!fx)
    {
        fx = (FX *)calloc(FX_N, sizeof(FX));
        mustInit(fx, "FX array");
    }

    for(int i = 0; i < FX_N; i++)
    {
        fx[i].used = false;
    }
}

void addFX(bool spark, int x, int y)
{
    if (!spark)
    {
        al_play_sample(sampleExplode, 0.2, 0.0, 4.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }

    for(int i = 0; i < FX_N; i++)
    {
        if(fx[i].used)
            continue;

        fx[i].x     = x;
        fx[i].y     = y;
        fx[i].frame = 0;
        fx[i].spark = 0;
        fx[i].used  = true;
        return;
    }
}

void updateFX()
{
    for(int i = 0; i < FX_N; i++)
    {
        if(!fx[i].used)
            continue;

        fx[i].frame++;

        if( ( !fx[i].spark && ( fx[i].frame == (EXPLOSION_FRAMES * 2) ) )
            || ( fx[i].spark && ( fx[i].frame == (SPARKS_FRAMES * 2) ) ) )
        {
            fx[i].used = false;
        }
    }
}

void drawFX()
{
    for(int i = 0; i < FX_N; i++)
    {
        if(!fx[i].used)
            continue;

        int frame_display = fx[i].frame / 2;
        ALLEGRO_BITMAP* bmp = fx[i].spark
            ? sprites->sparks[frame_display]
            : sprites->explosion[frame_display]
        ;

        int x = fx[i].x - (al_get_bitmap_width(bmp) / 2);
        int y = fx[i].y - (al_get_bitmap_height(bmp) / 2);
        al_draw_bitmap(bmp, x, y, 0);
    }
}

void deinitFX()
{
    free(fx);
}
