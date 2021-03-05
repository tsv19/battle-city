
#include "game-states.h"

STATE_GAME              state;
ALLEGRO_TIMER           *timer;
ALLEGRO_EVENT_QUEUE     *queue;
ALLEGRO_DISPLAY         *display;
ALLEGRO_BITMAP			*buffer;
ALLEGRO_FONT            *font;
unsigned char           key[ALLEGRO_KEY_MAX];


void stateInit()
{
    mustInit(al_init(), "allegro");
    mustInit(al_install_keyboard(), "keyboard");

    createTimer();
    
    createQueue();

    createDisplay();
    
    // audio

    mustInit(al_init_image_addon(), "image");
    initSprites();

    // hud

    mustInit(al_init_primitives_addon(), "primitives");


    initFont();

    initKeyboard();
    initTank();
    initShots();

    // createAudio(name, arq);

    // ALLEGRO_AUDIO_STREAM* music = al_load_audio_stream("music.opus", 2, 2048);
    // mustInit(music, "music");
    // al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);
    // al_attach_audio_stream_to_mixer(music, al_get_default_mixer());

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
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
                    done = true;
                }
                
                if(key[ALLEGRO_KEY_ENTER]){
                    state = SERVING;
                    done = true;
                }

                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                state = ENDGAME;
                done = true;
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
            al_draw_text(font, al_map_rgb(255, 255, 255), BUFFER_W/2, BUFFER_H/2, 
            ALLEGRO_ALIGN_CENTER, "TELA INICIAL PIKA");

            displayPostDraw();
            redraw = false;
        }
    }

}


void stateServe()
{
	state = PLAYING;
}


void statePlay()
{
	bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    for(;;)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                // faz update das coisas, posicoes etc
                updateTank();
                updateShots();

                if(key[ALLEGRO_KEY_ESCAPE])
                    done = true;

                redraw = true;
                // frames++;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                state = ENDGAME;
                done = true;
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

            drawTank();
            drawShots();
            
            displayPostDraw();

            redraw = false;
        }
    }

	// state = SERVING;
	state = GAMEOVER;
}


void stateGameOver()
{
	state = SERVING;
	state = ENDGAME;
}


void stateCloseGame()
{
	// al_destroy_audio_stream(music);
    // al_destroy_sample(elephant);
    deinitSprites();
    deinitFont();
    destroyDisplay();
    destroyTimer();
    destroyQueue();
	exit(0);
}