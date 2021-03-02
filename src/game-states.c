
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
    createTimer();
    
    createQueue();

    createFont();

    createDisplay();

    // mustInit(al_init_primitives_addon(), "primitives");

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

        keyboardUpdate(&event);

        if(redraw && al_is_event_queue_empty(queue))
        {
            // redesenha as coisas da tela

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), DISPLAY_W/2, DISPLAY_H/2, 
            ALLEGRO_ALIGN_CENTER, "TELA INICIAL PIKA");

            al_flip_display();

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
    keyboardInit();

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

        keyboardUpdate(&event);

        if(redraw && al_is_event_queue_empty(queue))
        {
            // redesenha as coisas da tela

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), DISPLAY_W/2, DISPLAY_H/2, 
            ALLEGRO_ALIGN_CENTER, "EH PRA SER O JOGO");

            al_flip_display();

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
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
	exit(0);
}