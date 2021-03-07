
#include "lib-game-states.h"

// variaveis globais externalizadas
ALLEGRO_TIMER           *timer;
ALLEGRO_EVENT_QUEUE     *queue;
ALLEGRO_DISPLAY         *display;
ALLEGRO_BITMAP			*buffer;
ALLEGRO_FONT            *font;
SPRITES_T               *sprites;
PLAYER_T                *tank;
unsigned char           key[ALLEGRO_KEY_MAX];
SHOT_T                  *shots;
MAP_T                   *map;

ALLEGRO_SAMPLE          *sampleShot;
ALLEGRO_SAMPLE          *sampleExplode[2];

// variaveis globais
long score_display;



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
        mustInit(sprites, "sprites");
    }

    sprites->sheet = al_load_bitmap("./resources/spritesheet.png");
    mustInit(sprites->sheet, "spritesheet");

    sprites->tank[UP]        = grabSprite(  0, 128, TANK_W, TANK_H);
    sprites->tank[DOWN]      = grabSprite( 64, 128, TANK_W, TANK_H);
    sprites->tank[LEFT]      = grabSprite( 32, 128, TANK_W, TANK_H);
    sprites->tank[RIGHT]     = grabSprite( 96, 128, TANK_W, TANK_H);

    sprites->life            = grabSprite(  0, 128, TANK_W, TANK_H);

    sprites->shot[UP]        = grabSprite(323, 102, SHOT_W, SHOT_H);
    sprites->shot[DOWN]      = grabSprite(339, 102, SHOT_W, SHOT_H);
    sprites->shot[LEFT]      = grabSprite(330, 102, SHOT_H, SHOT_W);
    sprites->shot[RIGHT]     = grabSprite(346, 102, SHOT_H, SHOT_W);

    sprites->enemies[UP]     = grabSprite(128, 144, TANK_W, TANK_H);
    sprites->enemies[DOWN]   = grabSprite(192, 144, TANK_W, TANK_H);
    sprites->enemies[LEFT]   = grabSprite(160, 144, TANK_W, TANK_H);
    sprites->enemies[RIGHT]  = grabSprite(224, 144, TANK_W, TANK_H);
 
    sprites->brickWall       = grabSprite(256,  64, BLOCKS, BLOCKS);
    sprites->steelWall       = grabSprite(256,  72, BLOCKS, BLOCKS);
    sprites->water           = grabSprite(256,  80, BLOCKS, BLOCKS);
    sprites->tree            = grabSprite(265,  72, BLOCKS, BLOCKS);
    sprites->contour         = grabSprite(265, 272, BLOCKS, BLOCKS);
}

ALLEGRO_BITMAP *grabSprite(int x, int y, int w, int h)
{
    ALLEGRO_BITMAP *sprite = al_create_sub_bitmap(sprites->sheet, x, y, w, h);
    mustInit(sprite, "sprite grab");
    return sprite;
}

void deinitSprites()
{
    al_destroy_bitmap(sprites->tank[RIGHT]);
    al_destroy_bitmap(sprites->tank[LEFT]);
    al_destroy_bitmap(sprites->tank[DOWN]);
    al_destroy_bitmap(sprites->tank[UP]);

    al_destroy_bitmap(sprites->life);

    al_destroy_bitmap(sprites->shot[RIGHT]);
    al_destroy_bitmap(sprites->shot[LEFT]);
    al_destroy_bitmap(sprites->shot[DOWN]);
    al_destroy_bitmap(sprites->shot[UP]);

    al_destroy_bitmap(sprites->enemies[RIGHT]);
    al_destroy_bitmap(sprites->enemies[LEFT]);
    al_destroy_bitmap(sprites->enemies[DOWN]);
    al_destroy_bitmap(sprites->enemies[UP]);

    al_destroy_bitmap(sprites->brickWall);
    al_destroy_bitmap(sprites->steelWall);
    al_destroy_bitmap(sprites->water);
    al_destroy_bitmap(sprites->tree);
    al_destroy_bitmap(sprites->contour);

    al_destroy_bitmap(sprites->sheet);

    // free(sprites);
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

    // sampleExplode[0] = al_load_sample("./resources/explode1.wav");
    // mustInit(sampleExplode[0], "explode sample 1");
    
    // sampleExplode[1] = al_load_sample("./resources/explode2.wav");
    // mustInit(sampleExplode[1], "explode sample 2");
}

void createAudio(ALLEGRO_SAMPLE *audio, char *name)
{
	audio = al_load_sample(name);
    mustInit(audio, "elephant");
}

void deinitAudio()
{
    al_destroy_sample(sampleShot);
    // al_destroy_sample(sampleExplode[0]);
    // al_destroy_sample(explodeSample[1]);
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
    al_play_sample(sampleShot, 0.3, 0, player ? 1.0 : betweenF(1.5, 1.6), 
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
        {
            al_draw_bitmap(sprites->shot[shots[i].direction], shots[i].x, 
                        shots[i].y, 0);
        }    
        else // enemy
        {
            ALLEGRO_COLOR tint = frame_display 
                        ? al_map_rgb_f(1, 1, 1) : al_map_rgb_f(0.5, 0.5, 0.5);

            al_draw_tinted_bitmap(sprites->shot[shots[i].direction], tint, 
                                shots[i].x, shots[i].y, 0);
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
        if(score_display <= (score - diff))
            score_display += diff;
    }
}

void drawHud()
{
    al_draw_textf(font, al_map_rgb_f(1,1,1), BUFFER_W - 50, 1, 0, "%06ld", score);

    int spacing = TANK_W + 1;
    for(int i = 0; i < tank->lives; i++)
    {
        al_draw_bitmap(sprites->life, (BUFFER_W - 50) + (i * spacing), 10, 0);
    }

}

/* ============================== hud game over ==============================*/
VET_SCORE_T *initHudGameOver()
{
    FILE* scores = fopen("./resources/highscores.txt", "r");
    mustInit(scores, "file of high scores");
    
    VET_SCORE_T *highScores = (VET_SCORE_T *) malloc(11 * sizeof(VET_SCORE_T));    
    mustInit(highScores, "High Scores");

    for(int i = 0; i < 10; i++)
    { 
        fscanf(scores, "%s %li", highScores[i].name, &highScores[i].score);
    }

    fclose(scores);
    return highScores;
}

void drawHudGameOver(VET_SCORE_T *highScores, char *str)
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

void deinitHudGameOver(VET_SCORE_T *highScores, char *str)
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
MAP_T *initMap()
{
    FILE  *mapGame;
    MAP_T *map;

    if (!map)
    {
        map = malloc(sizeof(MAP_T));
        mustInit(map, "map");
    }

    mapGame = fopen("map.dat", "r");
    if (!mapGame) {
        printf("Não foi possível abrir o arquivo mapa\n");
        exit(1);
    }

    if (fscanf(mapGame, "%d %d", &map->line, &map->column) != 2) {
        printf("Não foi possível ler o tamanho do mapa\n");
        exit(1);
    }

    if (!map->matrix)
    {
        map->matrix = malloc(sizeof(int *)*map->line);
        mustInit(map, "matrix map");
    }

    for (int i = 0; i < map->line; i++) {
        if (!map->matrix[i])
        {
            map->matrix[i] = malloc(sizeof(int)*map->column);
            mustInit(map->matrix[i], "line of matrix of map");
        }
    }

    for (int i = 0; i < map->line; i++) 
    {
        for (int j = 0; j < map->column; j++) 
        {
            mustInit(fscanf(mapGame, "%d", &map->matrix[i][j]), "map values");
        }
    }
    
    fclose(mapGame);
    return map;
}


void drawWalls(MAP_T *map)
{

    for (int i = 0; i < map->line; i++)
    {
        for (int j = 0; j < map->line; j++)
        {
            if (map->matrix[i][j] == 1)
            {
                al_draw_bitmap(sprites->contour, i * 16, j * 15, 0);

            }
        }
    }
}