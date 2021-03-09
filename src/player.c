
#include "player.h"

// lib-game-states
STATE_GAME state; 

// player
PLAYER_T *tank;

void initTank()
{
    if (!tank)
    {
        tank = (PLAYER_T *)malloc(sizeof(PLAYER_T));
    }

    tank->x = (BUFFER_W / 2) - (TANK_W / 2);
    tank->y = (BUFFER_H / 2) - (TANK_H / 2);
	tank->direction = UP;
    tank->shotTimer = 0;
    tank->lives = MAX_LIVES;
    tank->respawnTimer = 0;
    tank->invincibleTimer = INVINCIBLE_TIMER;
}


void updateTank()
{
    if(tank->respawnTimer)
    {
        tank->respawnTimer--;
        return;
    }

    if(key[ALLEGRO_KEY_LEFT])
	{
		tank->direction = LEFT;
        tank->x         -= TANK_SPEED;
	}
    if(key[ALLEGRO_KEY_RIGHT])
    {
		tank->direction = RIGHT;
	    tank->x         += TANK_SPEED;
    }
	if(key[ALLEGRO_KEY_UP])
    {
		tank->direction = UP;
	    tank->y         -= TANK_SPEED;
    }
	if(key[ALLEGRO_KEY_DOWN])
    {
		tank->direction = DOWN;
	    tank->y         += TANK_SPEED;
	}

    if(tank->x < 0)
        tank->x = 0;
    if(tank->y < 0)
        tank->y = 0;

    if(tank->x > TANK_MAX_X)
        tank->x = TANK_MAX_X;
    if(tank->y > TANK_MAX_Y)
        tank->y = TANK_MAX_Y;

    if(tank->invincibleTimer)
        tank->invincibleTimer--;
    else
    {
        if(collideShots(true, tank->x, tank->y, TANK_W, TANK_H))
        {
            int x = tank->x + (TANK_W / 2);
            int y = tank->y + (TANK_H / 2);
            addFX(false, x, y);

            tank->lives--;
            if (tank->lives > 0)
            {
                tank->respawnTimer       = RESPAWN_TIMER;
                tank->invincibleTimer    = INVINCIBLE_TIMER;
            }
        }
    }

    if (tank->lives > 0)
    {
        if(tank->shotTimer)
            tank->shotTimer--;
        else if(key[ALLEGRO_KEY_SPACE])
        {
            int x = tank->x + (TANK_W / 2);
            int y = tank->y + (TANK_H / 2);
            
            if(addShots(true, false, x, y, tank->direction))
                tank->shotTimer = SHOT_TIMER;
        }
    }
    else
    {
        state = GAMEOVER;
        return;
    }
}


void drawTank()
{
    if(tank->lives < 0)
        return;

    if(tank->respawnTimer)
        return;

    if(((tank->invincibleTimer / 2) % 3) == 1)
        return;

    al_draw_bitmap(sprites->tank[tank->direction], tank->x, tank->y, 0);
}


void deinitTank()
{
    free(tank);
} 
