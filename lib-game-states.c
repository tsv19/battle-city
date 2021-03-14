
#include "lib-game-states.h"

// lib-game-states
FX                   *fx;
MAP_T                *map;
long                 score;
ALLEGRO_FONT         *font;
PLAYER_T             *tank;
ALLEGRO_TIMER        *timer;
SHOT_T               *shots;
ALLEGRO_EVENT_QUEUE  *queue;
VET_BLOCK_T          *walls;
ALLEGRO_BITMAP       *buffer;
ALLEGRO_DISPLAY      *display;
SPRITES_T            *sprites;
bool                 easterEgg;
ALLEGRO_SAMPLE       *sampleShot;
ALLEGRO_SAMPLE       *sampleExplode;
unsigned char        key[ALLEGRO_KEY_MAX];

/* ================================ display ==================================*/

// faz a inicializacao do display
void createDisplay()
{
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    display = al_create_display(DISPLAY_W, DISPLAY_H);
    mustInit(display, "display");

    buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    mustInit(buffer, "bitmap buffer");
}


// desabilita o display
void destroyDisplay()
{
    al_destroy_bitmap(buffer);
    al_destroy_display(display);
}


// configuracoes do display antes do desenho.
// torna o buffer alvo dos desenhos
void displayPreDraw()
{
    al_set_target_bitmap(buffer);
}


// configuracoes do display apos o desenho ser feito no buffer.
// torna o display alvo e faz a escala do desenho feito no buffer para o display
void displayPostDraw()
{
    al_set_target_backbuffer(display);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISPLAY_W, 
    DISPLAY_H, 0);

    al_flip_display();
}


/* ================================ keybord ==================================*/

// inicializa teclado para receber dados
void initKeyboard()
{
    memset(key, 0, sizeof(key));
}


// atualiza dados vindo do teclado
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

// inicializa os sprites do jogo
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

    sprites->explosion[0] = grabSprite(sprites->sheet, 304, 129, 31, 30);
    sprites->explosion[1] = grabSprite(sprites->sheet, 335, 128, 32, 32);

    sprites->insignia = grabSprite(sprites->sheet, 304, 32, SIZE_BLOCKS * 2, SIZE_BLOCKS * 2);

    sprites->DK     = grabSprite(sprites->sheetEasterEgg1,   0, 0, 63, 56);
    sprites->banana = grabSprite(sprites->sheetEasterEgg2,   0, 0, 30, 31);
}


// pega os sprites de um spritesheet e os coloca em variaveis
ALLEGRO_BITMAP *grabSprite(ALLEGRO_BITMAP *sheet, int x, int y, int w, int h)
{
    ALLEGRO_BITMAP *sprite = al_create_sub_bitmap(sheet, x, y, w, h);
    mustInit(sprite, "sprite grab");
    return sprite;
}


// desabilita os sprites do jogo
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
    
    al_destroy_bitmap(sprites->insignia);

    al_destroy_bitmap(sprites->DK);
    al_destroy_bitmap(sprites->banana);

    al_destroy_bitmap(sprites->sheet);
    al_destroy_bitmap(sprites->sheetEasterEgg1);
    al_destroy_bitmap(sprites->sheetEasterEgg2);

    free(sprites);
}


/* ================================= timer ===================================*/

// inicializa o timer do jogo
void createTimer()
{
	timer = al_create_timer(1.0 / 60.0);
    mustInit(timer, "timer");
}


// desabilita o timer do jogo
void destroyTimer()
{
    al_destroy_timer(timer);
}


/* ================================= queue ===================================*/

// inicializa a fila de eventos
void createQueue()
{
	queue = al_create_event_queue();
    mustInit(queue, "queue");
}


// desabilita a fila de eventos
void destroyQueue()
{
    al_destroy_event_queue(queue);
}


/* ================================= audio ===================================*/

// inicializa os audios do jogo 
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


// desabilita audios do jogo
void deinitAudio()
{
    al_destroy_sample(sampleShot);
    al_destroy_sample(sampleExplode);
} 


/* ================================= shots ===================================*/

// inicializa variaveis de controle dos tiros do jogo
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


// adiciona os tiros em um vetor caso haja disparo
bool addShots(bool player, int x, int y, DIRECTIONS direction)
{
    al_play_sample(sampleShot, 0.1, 0, player ? 1.0 : betweenF(1.5, 1.6), 
                    ALLEGRO_PLAYMODE_ONCE, NULL);

    for(int i = 0; i < SHOTS_N; i++)
    {
        if(shots[i].used)
            continue;

        shots[i].player = player;

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
        shots[i].used = true;

        return true;
    }
    return false;
}


// atualiza os tiros que estao em tela
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


// confere a colisao dos tiros com os elementos do jogo
bool collideShots(bool player, bool list, int x, int y, int w, int h)
{
    for(int i = 0; i < SHOTS_N; i++)
    {
        if(!shots[i].used)
            continue;

        if(shots[i].player == player && !list)
            continue;

        int sw, sh;
        
        if (!easterEgg)
        {
            sw = SHOT_W;
            sh = SHOT_H;
        }
        else 
        {
            sw = BANANA_W;
            sh = BANANA_H;
        }

        if(collide(x, y, x+w, y+h, shots[i].x, shots[i].y, shots[i].x+sw, shots[i].y+sh))
        {
            if (!list)
                addFX(true, shots[i].x + (sw / 2), shots[i].y + (sh / 2));

            shots[i].used = false;
            return true;
        }
    }

    return false;
}


// desenha na tela os tiros
void drawShots()
{
    for(int i = 0; i < SHOTS_N; i++)
    {
        if(!shots[i].used)
            continue;

        int frameDisplay = (shots[i].frame / 2) % 2;

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
            ALLEGRO_COLOR tint = frameDisplay 
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


// desabilita os tiros do jogo
void deinitShots(){
    free(shots);
}


/* ================================== hud  ===================================*/

// inicializa o hud do jogo
void initHud()
{
    font = al_create_builtin_font();
    mustInit(font, "font");
}


// desenha na tela o hud
void drawHud()
{
    al_draw_text(font, al_map_rgb_f(1,1,1), GAME_LIMIT_W, 4, 0, "SCORE");
    al_draw_textf(font, al_map_rgb_f(1,1,1), GAME_LIMIT_W, 14, 0, "%06ld", score);

    int spacing = TANK_W + 1;
    for(int i = 0; i < tank->lives; i++)
    {
        al_draw_bitmap(sprites->life, (GAME_LIMIT_W) + (i * spacing), 24, 0);
    }

}


// desabilita o hud
void deinitHud()
{
    al_destroy_font(font);
}


/* ==================================  walls =================================*/

// inicializa o mapa do jogo com base em um arquivo "map.dat"
void initMap()
{
    FILE  *mapGame = fopen("./resources/map.txt", "r");
    mustInit(mapGame, "file map.txt");
    
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
    createWalls(map);
}


// cria as barreiras do jogo com base no arquivo lido
void createWalls(MAP_T *map)
{
    if (!walls)
    {
        walls = (VET_BLOCK_T *)malloc(sizeof(VET_BLOCK_T));
        mustInit(walls, "walls array");
    }

    walls->bricks = createList();
    mustInit(walls->bricks, "brick block list");
    
    walls->steel = createList();
    mustInit(walls->steel, "steel block list");
    
    walls->water = createList();
    mustInit(walls->water, "water block list");
    
    walls->tree = createList();
    mustInit(walls->tree, "tree block list");
    
    walls->contour = createList();
    mustInit(walls->contour, "contour block list");
    
    for (int i = 0; i < map->line; i++)
    {
        for (int j = 0; j < map->column; j++)
        {
            if (map->matrix[i][j] == BRICK)
            {
                addIndividualBlocks(walls->bricks, j * 16, i * 16);
            }

            else if (map->matrix[i][j] == STEEL)
            {
                addIndividualBlocks(walls->steel, j * 16, i * 16);
            }

            else if (map->matrix[i][j] == WATER)
            {
                addIndividualBlocks(walls->water, j * 16, i * 16);
            }

            else if (map->matrix[i][j] == TREE)
            {
                addIndividualBlocks(walls->tree, j * 16, i * 16);
            }

            else if (map->matrix[i][j] == CONTOUR)
            {
                addIndividualBlocks(walls->contour, j * 16, i * 16);
            }
        }
    }
}


// adiciona os blocks de forma individual para facilitar a destruicao por shots
void addIndividualBlocks(LIST_T *list, int x, int y)
{
    push(list, x, y);
    push(list, x, y + 8);
    push(list, x + 8, y);
    push(list, x + 8, y + 8);
}


// atualiza as barriras
void updateWalls()
{
    NODE_T *aux;
    
    aux = walls->bricks->head;
    while (aux != NULL)
    {
        if (collideShots(false, true, aux->x, aux->y, SIZE_BLOCKS, SIZE_BLOCKS))
        {
            NODE_T *temp = aux;
            aux = aux->next;
            pop(walls->bricks, temp->x, temp->y);
            break;
        }
        else
            aux = aux->next;
    }

    aux = walls->steel->head;
    while (aux != NULL)
    {
        if (collideShots(false, true, aux->x, aux->y, SIZE_BLOCKS, SIZE_BLOCKS))
            break;
        aux = aux->next;
    }

    aux = walls->contour->head;
    while (aux != NULL)
    {
        if (collideShots(false, true, aux->x, aux->y, SIZE_BLOCKS, SIZE_BLOCKS))
            break;
        aux = aux->next;
    }
}


// desenha as barreiras do jogo na tela
void drawMap()
{
    NODE_T *aux;
    
    aux = walls->bricks->head;
    while (aux != NULL)
    {
        al_draw_bitmap(sprites->block[BRICK], aux->x, aux->y, 0);
        aux = aux->next;
    }
    
    aux = walls->steel->head;
    while (aux != NULL)
    {
        al_draw_bitmap(sprites->block[STEEL], aux->x, aux->y, 0);
        aux = aux->next;
    }

    aux = walls->water->head;
    while (aux != NULL)
    {
        al_draw_bitmap(sprites->block[WATER], aux->x, aux->y, 0);
        aux = aux->next;
    }

    aux = walls->contour->head;
    while (aux != NULL)
    {
        al_draw_bitmap(sprites->block[CONTOUR], aux->x, aux->y, 0);
        aux = aux->next;
    }

    al_draw_bitmap(sprites->insignia, BASE_INIT_W, BASE_INIT_H, 0);
}


// faz o controle das colisoes com blocos de parede
bool collideControl(int x, int y, int w, int h, DIRECTIONS direction)
{
    if (collideWalls(walls->bricks, x, y, w, h, direction))
        return true;

    if (collideWalls(walls->steel, x, y, w, h, direction))
        return true;

    if (collideWalls(walls->contour, x, y, w, h, direction))
        return true;

    return false;
}


// confere a colisao com as paredes
bool collideWalls(LIST_T *list, int x, int y, int w, int h, DIRECTIONS direction)
{
    NODE_T *aux = list->head;

    int ax1 = x;
    int ax2 = x + w;
    int ay1 = y;
    int ay2 = y + h;

    int bx1, bx2, by1, by2;

    while (aux != NULL)
    {   
        bx1 = aux->x + 2;
        bx2 = aux->x + SIZE_BLOCKS - 2;
        by1 = aux->y + 2;
        by2 = aux->y + SIZE_BLOCKS - 2;

        if (direction == UP && (ay1 == by2) && !(ax1 > bx2) && !(ax2 < bx1))
        {
            return true;
        }
        else if (direction == DOWN && (ay2 == by1) && !(ax1 > bx2) && !(ax2 < bx1))
        {
            return true;
        }
        else if (direction == LEFT && (ax1 == bx2) && !(ay1 > by2) && !(ay2 < by1))
        {
            return true;
        }
        else if (direction == RIGHT && (ax2 == bx1) && !(ay1 > by2) && !(ay2 < by1))
        {
            return true;
        }
        
        aux = aux->next;
    }
    
    return false;
}


// confere a colisao com a agua
bool collideWater(int x, int y, int w, int h)
{
    NODE_T *aux;
    
    aux = walls->water->head;
    while (aux != NULL)
    {
        if (collide(x, y, x + w, y + h, aux->x + 2, aux->y + 2, aux->x + SIZE_BLOCKS - 2, 
            aux->y + SIZE_BLOCKS - 2))
            return true;
        
        aux = aux->next;
    }

    return false;
}


// desenha as arvores do jogo
void drawTrees()
{
    NODE_T *aux;

    aux = walls->tree->head;
    while (aux != NULL)
    {
        al_draw_bitmap(sprites->block[TREE], aux->x, aux->y, 0);
        aux = aux->next;
    }
}


// desabilita o mapa
void deinitMap()
{
    destroyList(walls->bricks);
    destroyList(walls->steel);
    destroyList(walls->water);
    destroyList(walls->tree);
    destroyList(walls->contour);
    free(map);
}


/* ============================== easter egg ================================ */

// checa se o easter egg foi descoberto
bool checkIfEasterEgg(char *str)
{
    if (strstr(str, "kong") != NULL)
    {
        return easterEgg = true;
    }

    return false;
}


// desenha easter egg do menu
void drawEasterEgg()
{
    al_draw_bitmap(sprites->banana, BUFFER_W/1.5 + 30, BUFFER_H/1.5, 0);
    al_draw_bitmap(    sprites->DK,              30,         20, 0);
}


/* ========================== FX, explosion, etc ============================ */

// inicializa os "efeitos especiais"
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


// adiciona explosao em um vetor caso haja destruicao de inimigo
void addFX(bool spark, int x, int y)
{
    if (!spark)
    {
        al_play_sample(sampleExplode, 0.1, 0.0, 4.0, ALLEGRO_PLAYMODE_ONCE, NULL);
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


// atualiza o andamento da explosao
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


// desenha a explosao
void drawFX()
{
    for(int i = 0; i < FX_N; i++)
    {
        if(!fx[i].used)
            continue;

        int frameDisplay = fx[i].frame / 2;
        ALLEGRO_BITMAP* bmp = fx[i].spark
            ? sprites->sparks[frameDisplay]
            : sprites->explosion[frameDisplay]
        ;

        int x = fx[i].x - (al_get_bitmap_width(bmp) / 2);
        int y = fx[i].y - (al_get_bitmap_height(bmp) / 2);
        al_draw_bitmap(bmp, x, y, 0);
    }
}


// desabilita as explosoes
void deinitFX()
{
    free(fx);
}
