
#include "game-states.h"

// lib-game-states
ALLEGRO_FONT        *font;
STATE_GAME          state;
long                score;
long                frames;
ALLEGRO_TIMER       *timer;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_BITMAP		*buffer;
ALLEGRO_DISPLAY     *display;
unsigned char       key[ALLEGRO_KEY_MAX];


// estado de inicializacao. realiza as configuracoes iniciais e exibe a 
// tela inicial para o jogador
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

    char str[1024];
    strcpy(str, "");

    bool redraw  = true;
    bool done    = false;
	bool help    = false;
	bool credits = false;
    ALLEGRO_EVENT event;

    al_start_timer(timer);    
	for(;;)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:

                // fecha a janela se "ESC" for pressionado
                if(key[ALLEGRO_KEY_ESCAPE]){
                    state = ENDGAME;
                    done  = true;
                }

                // muda para o estado de "jogar" se "ENTER" for pressionado
                if(key[ALLEGRO_KEY_ENTER]){
                    state = SERVING;
                    done  = true;
                }

                redraw = true;
                break;

            // faz a checagem das teclas pressionadas para validar easter egg e
            // outros menus 
            case ALLEGRO_EVENT_KEY_CHAR:
            
                // se letra "h" for pressionada, abre menu de ajuda
                if (event.keyboard.keycode == ALLEGRO_KEY_H 
                    || event.keyboard.keycode == ALLEGRO_KEY_F11)
                {
                    help = !help;
                }

                // se letra "c" for pressionada, abre menu de creditos
                if (event.keyboard.keycode == ALLEGRO_KEY_C)
                {
                    credits = !credits;
                }
                // TRECHO DE CODIGO RETIRADO DE
                // https://gist.github.com/rafaeltoledo/6e157f233e94168d1af0fb2ee988cc19#file-tutorial14-c
                // verifica se o teclado escreveu um caractere utilizavel
                else if (strlen(str) < 1024)
                {
                    if (strlen(str) == 1023)
                    {
                        strcpy(str, "");
                    }

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
                //FIM DO TRECHO RETIRADO

                redraw = true;
                break;

            // verifica se a janela foi fechada 
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
            // pre configuracoes de desenho na tela
            displayPreDraw();
            al_clear_to_color(al_map_rgb(0, 0, 0));
            

            // tela inicial
            if (!help && !credits)
            {
                if (checkIfEasterEgg(str))
                {
                    drawEasterEgg();
                }

                al_draw_text(font, al_map_rgb(255, 50, 100), BUFFER_W/2, 
                            BUFFER_H/3, ALLEGRO_ALIGN_CENTER, "BATTLE CITY");

                al_draw_text(font, al_map_rgb(255, 50, 100), BUFFER_W/2, 
                            BUFFER_H/2, ALLEGRO_ALIGN_CENTER, "PLAY [ENTER]");
                
                al_draw_text(font, al_map_rgb(255, 50, 100), BUFFER_W/2, 
                            BUFFER_H/2 + 20, ALLEGRO_ALIGN_CENTER, "HELP [H][F1]");

                al_draw_text(font, al_map_rgb(255, 50, 100), BUFFER_W/2, 
                            BUFFER_H/2 + 40, ALLEGRO_ALIGN_CENTER, "CREDITS [C]");

                al_draw_text(font, al_map_rgb(255, 50, 100), BUFFER_W/2, 
                            BUFFER_H/2 + 60, ALLEGRO_ALIGN_CENTER, "EXIT [ESC]");
            }
            // tela de ajuda
            else if (help)
            {
                helpScreen();
            }
            // tela de creditos
            else if (credits)
            {
                creditScreen();
            }

            // configuracoes pos desenho na tela
            displayPostDraw();
            redraw = false;
        }
    }
}


// estado de servico. realiza a preparacao dos elementos principais para 
// haver um jogo 
void stateServe()
{
    score = 0;
    initMap();
    initFX();
    initTank();
    initEnemies();
    initShots();
    state = PLAYING;
}


// estado de play. realiza o controle do jogo, onde o jogo eh executado
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
                
                // atualiza os elementos do jogo
                updateTank();
                updateEnemies(&score);
                updateFX();
                updateShots();
                updateWalls();

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
            displayPreDraw();
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // desenha os elementos do jogo
            drawMap();
            drawTank();
            drawEnemies();
            drawFX();
            drawShots();
            drawTrees();
            drawHud(&score);

            displayPostDraw();
            redraw = false;
        }
    }
}


// estado de game over. apos o jogo ser perdido, exibe uma tela onde havera o
// score do jogador, asmaiores pontuacoes e a possibilidade de adicionar o nome
// para salvar na pontuacao
void stateGameOver()
{
    bool redraw  = true;
    bool done    = false;
    ALLEGRO_EVENT event;

    // inicializa tela de game over
    initScreenGameOver();

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
                // CODIGO RETIRADO DE 
                // https://gist.github.com/rafaeltoledo/6e157f233e94168d1af0fb2ee988cc19#file-tutorial14-c
                // verifica se o teclado escreveu uma caractere utilizavel
                else if (strlen(str) < 10)
                {
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
                //FIM DO TRECHO RETIRADO

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
            displayPreDraw();
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // desenha tela de game over
            drawScreenGameOver(str);

            displayPostDraw();
            redraw = false;
        }
    }

    // desabilita tela de game over e mapa
    deinitScreenGameOver(str);
    deinitMap();
}


// estado de fechamento do jogo. desabilita as configuracoes e encerra o jogo
void stateCloseGame()
{
    deinitAudio();
    deinitEnemies();
    deinitFX();
    deinitTank();
    deinitSprites();
    deinitHud();
    destroyDisplay();
    destroyTimer();
    destroyQueue();
}
