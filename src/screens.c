
#include "screens.h" 

// lib-game-states variables
ALLEGRO_FONT            *font;
SPRITES_T               *sprites;
long 					score;

// screens variables
VET_SCORE_T             *highScores;


/* =========================== screen game over ==============================*/

// inicializa a tela de game over 
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


// desenha a tela de game over
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


// desabilita a tela de game over
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


/* =============================== help screen ============================== */

// configuracoes da tela de ajuda
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
                "<- You can destroy if you shot it");

    al_draw_bitmap(sprites->block[STEEL], 15, 72, 0);
    al_draw_text(font, al_map_rgb(255, 255, 255), 30, 72, 0, 
                "<- You can't destroy if you shot it");

    al_draw_bitmap(sprites->block[WATER], 15, 92, 0);
    al_draw_text(font, al_map_rgb(255, 255, 255), 30, 92, 0, 
                "<- Water + tank = dead");

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

// configuracoes da tela de creditos
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

