
#include "main.h"

STATE_GAME state;

int main ()
{
	mustInit(al_init(), "allegro");
    mustInit(al_install_keyboard(), "keyboard");

	state = START;
	for (;;)
		switch (state)
		{
			case START		: stateInit()		;  	break;
			case SERVING	: stateServe()		; 	break;
			case PLAYING	: statePlay() 		;  	break;
			case GAMEOVER	: stateGameOver() 	;  	break;
			case ENDGAME	: stateCloseGame()	;	return 0;
			default: break ;
		}
}