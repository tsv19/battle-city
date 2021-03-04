
#include "player.h"

PLAYER_T tank;

// variaveis globais externalizadas

void initTank()
{
    tank.x = (BUFFER_W / 2) - (TANK_W / 2);
    tank.y = (BUFFER_H / 2) - (TANK_H / 2);
	tank.direction = UP;
    tank.shotTimer = 0;
    tank.lives = 3;
    tank.respawnTimer = 0;
    tank.invincibleTimer = 120;
}


void updateTank()
{
    if(tank.lives < 0)
        return;

    if(tank.respawnTimer)
    {
        tank.respawnTimer--;
        return;
    }

    if(key[ALLEGRO_KEY_LEFT])
	{
        tank.x -= TANK_SPEED;
		tank.direction = LEFT;
	}
    if(key[ALLEGRO_KEY_RIGHT])
    {
	    tank.x += TANK_SPEED;
		tank.direction = RIGHT;
    }
	if(key[ALLEGRO_KEY_UP])
    {
	    tank.y -= TANK_SPEED;
		tank.direction = UP;
    }
	if(key[ALLEGRO_KEY_DOWN])
    {
	    tank.y += TANK_SPEED;
		tank.direction = DOWN;
	}

    if(tank.x < 0)
        tank.x = 0;
    if(tank.y < 0)
        tank.y = 0;

    if(tank.x > TANK_MAX_X)
        tank.x = TANK_MAX_X;
    if(tank.y > TANK_MAX_Y)
        tank.y = TANK_MAX_Y;

    // if(tank.invincibleTimer)
        // tank.invincibleTimer--;
    // else
    // {
    //     if(shots_collide(true, tank.x, tank.y, TANK_W, TANK_H))
    //     {
    //         int x = tank.x + (TANK_W / 2);
    //         int y = tank.y + (TANK_H / 2);
    //         fx_add(false, x, y);
    //         fx_add(false, x+4, y+2);
    //         fx_add(false, x-2, y-4);
    //         fx_add(false, x+1, y-5);

    //         tank.lives--;
    //         tank.respawnTimer = 90;
    //         tank.invincibleTimer = 180;
    //     }
    // }

    // if(tank.shotTimer)
        // tank.shotTimer--;
    // else if(key[ALLEGRO_KEY_X])
    // {
    //     int x = tank.x + (TANK_W / 2);
        // if(shots_add(true, false, x, tank.y))
    //         tank.shotTimer = 5;
    // }
}


void drawTank()
{
    if(tank.lives < 0)
        return;
    if(tank.respawnTimer)
        return;
    if(((tank.invincibleTimer / 2) % 3) == 1)
        return;

    al_draw_bitmap(sprites.tank[tank.direction], tank.x, tank.y, 0);
}