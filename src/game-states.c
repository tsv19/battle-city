
#include "game-states.h"

STATE_GAME              state;
ALLEGRO_TIMER           *timer;
ALLEGRO_EVENT_QUEUE     *queue;
ALLEGRO_DISPLAY         *display;
ALLEGRO_BITMAP			*buffer;
ALLEGRO_FONT            *font;
unsigned char           key[ALLEGRO_KEY_MAX];
long score;
long frames;
MAP_T                   *map;

void stateInit()
{
    mustInit(al_init(), "allegro");
    mustInit(al_install_keyboard(), "keyboard");
    mustInit(al_init_image_addon(), "image");
    mustInit(al_init_primitives_addon(), "primitives");

    createTimer();
    createQueue();
    createDisplay();
    initKeyboard();
    initHud();
    initAudio();
    initSprites();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done   = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);    
	for(;;)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                // faz update das coisas, posicoes etc
                if(key[ALLEGRO_KEY_ESCAPE]){
                    state = ENDGAME;
                    done  = true;
                }
                
                if(key[ALLEGRO_KEY_ENTER]){
                    state = SERVING;
                    done  = true;
                }

                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                state = ENDGAME;
                done  = true;
                break;
        }

        if(done)
            break;

        updateKeyboard(&event);

        if(redraw && al_is_event_queue_empty(queue))
        {
            // redesenha as coisas da tela
            displayPreDraw();

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 50, 100), BUFFER_W/2, BUFFER_H/3, 
            ALLEGRO_ALIGN_CENTER, "BATTLE CITY");

            al_draw_text(font, al_map_rgb(255, 50, 100), BUFFER_W/2, BUFFER_H/2, 
            ALLEGRO_ALIGN_CENTER, "PLAY [ENTER]");
            
            al_draw_text(font, al_map_rgb(255, 50, 100), BUFFER_W/2, BUFFER_H/2 + 20, 
            ALLEGRO_ALIGN_CENTER, "HELP [H]");

            displayPostDraw();
            redraw = false;
        }
    }
}


void stateServe()
{
    score = 0;
    map = initMap();
    initTank();
    initEnemies();
    initShots();
    state = PLAYING;
}


void statePlay()
{
    bool redraw = true;
	bool done   = false;
    ALLEGRO_EVENT event;

    for(;;)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                // faz update das coisas, posicoes etc
                updateTank();
                updateEnemies();
                updateShots();
                updateHud();

                if(key[ALLEGRO_KEY_ESCAPE])
                    state = ENDGAME;

                if (state == GAMEOVER || state == ENDGAME)
                    done  = true;

                redraw = true;
                frames++;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                state = ENDGAME;
                done  = true;
                break;
        }

        if(done)
            break;

        updateKeyboard(&event);

        if(redraw && al_is_event_queue_empty(queue))
        {
            // redesenha as coisas da tela
            displayPreDraw();
            al_clear_to_color(al_map_rgb(0, 0, 0));

            drawWalls(map);
            drawTank();
            drawEnemies();
            drawShots();
            drawHud();

            displayPostDraw();
            redraw = false;
        }
    }
}


void stateGameOver()
{
    bool redraw  = true;
    bool done    = false;
    ALLEGRO_EVENT event;


    VET_SCORE_T *highScores = initHudGameOver();

    char str[10]; 
    strcpy(str, "");

	for(;;)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_CHAR:
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                {
                    state = ENDGAME;
                    done  = true;
                }
                
                if (event.keyboard.keycode == ALLEGRO_KEY_ENTER)
                {
                    state = SERVING;
                    done  = true;
                }
                else if (strlen(str) <= 13){
                    //  INICIO DO CODIGO RETIRADO DE https://gist.github.com/rafaeltoledo/6e157f233e94168d1af0fb2ee988cc19#file-tutorial14-c
                    //  o codigo verifica se o teclado escreveu uma caractere utilizavel
                    char temp[] = {event.keyboard.unichar, '\0'};
                    
                    if (event.keyboard.unichar >= '0' 
                        && event.keyboard.unichar <= '9')
                    {
                        strcat(str, temp);
                    }
                    else if (event.keyboard.unichar >= 'A' 
                        && event.keyboard.unichar <= 'Z')
                    {
                        strcat(str, temp);
                    }
                    else if (event.keyboard.unichar >= 'a' 
                        && event.keyboard.unichar <= 'z')
                    {
                        strcat(str, temp);
                    }
                }
                //FIM DO CODIGO RETIRADO 
                redraw = true;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                state = ENDGAME;
                done  = true;
                break;
        }

        if(done)
            break;

        updateKeyboard(&event);

        if(redraw && al_is_event_queue_empty(queue))
        {
            // redesenha as coisas da tela
            displayPreDraw();
            al_clear_to_color(al_map_rgb(0, 0, 0));

            drawHudGameOver(highScores, str);

            displayPostDraw();
            redraw = false;
        }
    }

    deinitHudGameOver(highScores, str);
}


void stateCloseGame()
{
    deinitAudio();
    deinitEnemies();
    deinitTank();
    deinitSprites();
    deinitHud();
    destroyDisplay();
    destroyTimer();
    destroyQueue();
	exit(0);
}
