
#include "player.h"

// lib-game-states varaibles
STATE_GAME  state; 

// player variables
PLAYER_T *tank;


// inicializa tank do player
void initTank()
{
    if (!tank)
    {
        tank = (PLAYER_T *)malloc(sizeof(PLAYER_T));
    }

    tank->x = TANK_INIT_W;
    tank->y = TANK_INIT_H;
	tank->direction = UP;
    tank->shotTimer = 0;
    tank->lives = MAX_LIVES;
    tank->respawnTimer = 0;
    tank->invincibleTimer = INVINCIBLE_TIMER;
}


// atualiza o tank do player
void updateTank()
{
    // tempo de respawn
    if(tank->respawnTimer)
    {
        tank->respawnTimer--;
        return;
    }

    // checa se a movimentacao para a direcao escolhida eh valida
    if(key[ALLEGRO_KEY_LEFT] 
        && !collideControl(tank->x-TANK_SPEED, tank->y, TANK_W-1, TANK_H-1, LEFT)
    )
	{
		tank->direction = LEFT;
        tank->x         -= TANK_SPEED;
	}
    else if (key[ALLEGRO_KEY_RIGHT]  
        && !collideControl(tank->x+TANK_SPEED, tank->y, TANK_W-1, TANK_H-1, RIGHT)
    )
    {
		tank->direction = RIGHT;
	    tank->x         += TANK_SPEED;
    }
	else if (key[ALLEGRO_KEY_UP] 
        && !collideControl(tank->x, tank->y-TANK_SPEED, TANK_W-1, TANK_H-1, UP)
    )
    {
		tank->direction = UP;
	    tank->y         -= TANK_SPEED;
    }
	else if (key[ALLEGRO_KEY_DOWN] 
        && !collideControl(tank->x, tank->y+TANK_SPEED, TANK_W-1, TANK_H-1, DOWN)
    )
    {
		tank->direction = DOWN;
	    tank->y         += TANK_SPEED;
	}

    // checa se a base foi destruida
    if (collide(tank->x, tank->y, tank->x+TANK_W, tank->y+TANK_H-2, BASE_INIT_W, 
        BASE_INIT_H+2, BASE_INIT_W+TANK_W, BASE_INIT_H+TANK_H))
        state = GAMEOVER;

    if(tank->invincibleTimer)
        tank->invincibleTimer--;
    else
    {
        // checa colisao com tiros e agua
        if(collideShots(true, false, tank->x, tank->y, TANK_W, TANK_H)
            || collideWater(tank->x, tank->y, TANK_W - 1, TANK_H - 1))
        {
            int x = tank->x + (TANK_W / 2);
            int y = tank->y + (TANK_H / 2);
            addFX(false, x, y);

            tank->lives--;
            if (tank->lives > 0)
            {
                tank->x               = TANK_INIT_W;
                tank->y               = TANK_INIT_H;
                tank->respawnTimer    = RESPAWN_TIMER;
                tank->invincibleTimer = INVINCIBLE_TIMER;
            }
        }
    }

    // se tiver vivo, checa se pode atirar
    if (tank->lives > 0)
    {
        if(tank->shotTimer)
            tank->shotTimer--;
        else if(key[ALLEGRO_KEY_SPACE])
        {
            int x = tank->x + (TANK_W / 2);
            int y = tank->y + (TANK_H / 2);
            
            if(addShots(true, x, y, tank->direction))
                tank->shotTimer = SHOT_TIMER;
        }
    }
    else
    {
        state = GAMEOVER;
        return;
    }
}


// desenha o tank do player
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


// desabilita tank do player
void deinitTank()
{
    free(tank);
} 
