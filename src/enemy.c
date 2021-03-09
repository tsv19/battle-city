
#include "enemy.h"

long frames;
long score;
ENEMY_T *enemies;

void initEnemies()
{
    if (!enemies)
    {
        enemies = (ENEMY_T *)malloc(sizeof(ENEMY_T) * ENEMY_N);
    }

    for(int i = 0; i < ENEMY_N; i++)
        enemies[i].used = false;

    for(int i = 0; i < ENEMY_N; i++)
        enemies[i].alive = true;
}

void updateEnemies()
{
    int new_quota = (frames % 120) ? 0 : between(2, 4);
    
	int new_x = between(0, GAME_LIMIT_W);

    for(int i = 0; i < ENEMY_N; i++)
    {
        if(!enemies[i].used && enemies[i].alive)
        {
            // if this enemy is unused, should it spawn?
            if(new_quota > 0)
            {
                new_x += between(40, 80);
                if(new_x > (GAME_LIMIT_W))
                    new_x -= (GAME_LIMIT_W - 60);

				enemies[i].direction = between(0, DIRECTIONS_N - 1);
                enemies[i].used      = true;
                enemies[i].x         = new_x;
                enemies[i].y         = between(0, 32);

                new_quota--;
            }
            continue;
        }

        if (enemies[i].alive)
        {
            if (between(0, 10000) < TIME_MOVEMENT)
                enemies[i].direction = between(0, DIRECTIONS_N - 1);

            if (enemies[i].direction == UP)
            {
                enemies[i].y -= TANK_SPEED;
                if (enemies[i].y < 0)
                    enemies[i].y = 0;
            }
            if (enemies[i].direction == DOWN)
            {
                enemies[i].y += TANK_SPEED;
                if (enemies[i].y > TANK_MAX_Y)
                    enemies[i].y = TANK_MAX_Y;
            }
            if (enemies[i].direction == LEFT)
            {
                enemies[i].x -= TANK_SPEED;
                if (enemies[i].x < 0)
                    enemies[i].x = 0;
            }
            if (enemies[i].direction == RIGHT)
            {
                enemies[i].x += TANK_SPEED;
                if (enemies[i].x > TANK_MAX_X)
                    enemies[i].x = TANK_MAX_X;
            }

            if(enemies[i].y >= BUFFER_H)
            {
                enemies[i].used = true;
                continue;
            }

            int cx = enemies[i].x + (TANK_W / 2);
            int cy = enemies[i].y + (TANK_H / 2);

            if(collideShots(false, enemies[i].x, enemies[i].y, TANK_W, TANK_H))
            {
                enemies[i].alive = false;
                enemies[i].used  = true;
                
                score += 200;

                addFX(false, cx, cy);

                continue;
            }

            if(between(0, 10000) < SHOT_TIMER)
            {
                addShots(false, false, cx, cy, enemies[i].direction);
            }
        }
    }
}

void drawEnemies()
{
    for(int i = 0; i < ENEMY_N; i++)
    {
        if(!enemies[i].used)
            continue;

        if (enemies[i].alive)
            al_draw_bitmap(sprites->enemies[enemies[i].direction], enemies[i].x, 
		                enemies[i].y, 0);
    }
}

void deinitEnemies()
{
    free(enemies);
}