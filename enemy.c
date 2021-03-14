
#include "enemy.h"

// lib-game-states variables 
long        frames;
long        score;
SPRITES_T   *sprites;

// enemy variables
ENEMY_T     *enemies;
int         enemiesAlives;


// inicializa os inimigos do jogo
void initEnemies()
{
    if (!enemies)
    {
        enemies = (ENEMY_T *)malloc(sizeof(ENEMY_T) * ENEMY_N);
        mustInit(enemies, "enemies");
    }

    for(int i = 0; i < ENEMY_N; i++)
        enemies[i].used = false;

    enemiesAlives = 0;
}


// atualiza o estado dos inimigos
void updateEnemies()
{
    int newQuota = (frames % 120) ? 0 : between(2, 4);
    
	int newX = between(TANK_W, GAME_LIMIT_W - 2 * TANK_W);

    for (int i = 0; i < ENEMY_N; i++)
    {
        if (!enemies[i].used)
        {
            // se o inimigo nao estiver sendo usado e tem menos de 5 inimigos na
            // tela
            if (newQuota > 0 && enemiesAlives < 5)
            {
                newX += between(TANK_W, GAME_LIMIT_W - 2 * TANK_W);
                if(newX > (GAME_LIMIT_W - 2 * TANK_W))
                    newX -= (GAME_LIMIT_W - 2 * TANK_W);

                enemies[i].used      = true;
                enemies[i].x         = newX;
                enemies[i].y         = TANK_W;
                enemies[i].shotTimer = SHOT_TIMER;
				enemies[i].direction = between(0, DIRECTIONS_N);

                enemiesAlives++;
                newQuota--;
            }
            continue;
        }

        // determina a direcao do proximo movimento
        if (between(0, 5000) < TIME_MOVEMENT)
            enemies[i].direction = between(0, DIRECTIONS_N);
        
        // determina o proximo movimento
        nextMove(&enemies[i]);

        // verifica se a base foi invadida por inimigos
        if (collide(enemies[i].x, enemies[i].y, enemies[i].x+TANK_W, 
            enemies[i].y+TANK_H-2, BASE_INIT_W, BASE_INIT_H+2, 
            BASE_INIT_W+TANK_W, BASE_INIT_H+TANK_H))
            state = GAMEOVER;

        int cx = enemies[i].x + (TANK_W / 2);
        int cy = enemies[i].y + (TANK_H / 2);

        // determina a hora de atirar
        enemies[i].shotTimer--;
        if (between(0, 1000) < SHOT_TIMER && enemies[i].shotTimer <= 0)
        {
            if (addShots(false, cx, cy, enemies[i].direction))
                enemies[i].shotTimer = SHOT_TIMER;
        }

        // verifica se houve colisao com os tiros
        if (collideShots(false, false, enemies[i].x, enemies[i].y, TANK_W, TANK_H))
        {
            enemies[i].used  = false;
            
            score += 200;

            addFX(false, cx, cy);

            enemiesAlives--;
            continue;
        }

        // verifica se houve colisao com a agua
        if (collideWater(enemies[i].x, enemies[i].y, TANK_W-1, TANK_H-1))
        {
            enemies[i].used  = false;

            addFX(false, cx, cy);

            enemiesAlives--;
            continue;
        }
    }
}


// realiza a movimentacao do inimigo
bool nextMove(ENEMY_T *enemies)
{
    if (enemies->direction == UP && 
        !collideControl(enemies->x, enemies->y-TANK_SPEED, TANK_W-1, TANK_H-1, UP)
    )
    {
        enemies->y -= TANK_SPEED;
        if (enemies->y < 0)
            enemies->y = 0;
        return true;
    }
    if (enemies->direction == DOWN && 
        !collideControl(enemies->x, enemies->y+TANK_SPEED, TANK_W-1, TANK_H-1, DOWN)
    )
    {
        enemies->y += TANK_SPEED;
        if (enemies->y > TANK_MAX_Y)
            enemies->y = TANK_MAX_Y;
        return true;
    }
    if (enemies->direction == LEFT && 
        !collideControl(enemies->x-TANK_SPEED, enemies->y, TANK_W-1, TANK_H-1, LEFT)
    )
    {
        enemies->x -= TANK_SPEED;
        if (enemies->x < 0)
            enemies->x = 0;
        return true;
    }
    if (enemies->direction == RIGHT && 
        !collideControl(enemies->x+TANK_SPEED, enemies->y, TANK_W-1, TANK_H-1, RIGHT)
    )
    {
        enemies->x += TANK_SPEED;
        if (enemies->x > TANK_MAX_X)
            enemies->x = TANK_MAX_X;
        return true;
    }

    return false;
}


// desenha os inimigos na tela
void drawEnemies()
{
    for(int i = 0; i < ENEMY_N; i++)
    {
        if(!enemies[i].used)
            continue;

        al_draw_bitmap(sprites->enemies[enemies[i].direction], enemies[i].x, 
                    enemies[i].y, 0);
    }
}


// desabilita os inimigos
void deinitEnemies()
{
    free(enemies);
}