
#include "lib-game-states.h"

ALLEGRO_TIMER           *timer;
ALLEGRO_EVENT_QUEUE     *queue;
ALLEGRO_DISPLAY         *display;
ALLEGRO_BITMAP			*buffer;
ALLEGRO_FONT            *font;
unsigned char key[ALLEGRO_KEY_MAX];


/* ================================ display ==================================*/
void createDisplay()
{
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    display = al_create_display(DISPLAY_W, DISPLAY_H);
    mustInit(display, "display");

    buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    mustInit(buffer, "bitmap buffer");
}


void destroyDisplay()
{
    al_destroy_bitmap(buffer);
    al_destroy_display(display);
}


void displayPreDraw()
{
    al_set_target_bitmap(buffer);
}


void displayPostDraw()
{
    al_set_target_backbuffer(display);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISPLAY_W, 
    DISPLAY_H, 0);

    al_flip_display();
}


/* ================================ keybord ==================================*/
void keyboardInit()
{
    memset(key, 0, sizeof(key));
}


void keyboardUpdate(ALLEGRO_EVENT *event)
{
    switch(event->type)
    {
        case ALLEGRO_EVENT_TIMER:
            for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                key[i] &= KEY_SEEN;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            key[event->keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[event->keyboard.keycode] &= KEY_RELEASED;
            break;
    }
}


void createTimer()
{
	timer = al_create_timer(1.0 / 60.0);
    mustInit(timer, "timer");
}


void createQueue()
{
	queue = al_create_event_queue();
    mustInit(queue, "queue");
}


void createFont()
{
	font = al_create_builtin_font();
    mustInit(font, "font");
}


void initAudio()
{
	mustInit(al_install_audio(), "audio");
    mustInit(al_init_acodec_addon(), "audio codecs");
    mustInit(al_reserve_samples(16), "reserve samples");
}


void createAudio(ALLEGRO_SAMPLE *audio, char *name)
{
	audio = al_load_sample(name);
    mustInit(audio, "elephant");
}



