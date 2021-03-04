
#include "main.h"

STATE_GAME state;

int main ()
{
	state = START;
	for (;;)
		switch (state)
		{
			case START		: stateInit()		;  	break;
			case SERVING	: stateServe()		; 	break;
			case PLAYING	: statePlay() 		;  	break;
			case GAMEOVER	: stateGameOver() 	;  	break;
			case ENDGAME	: stateCloseGame()	;	exit(0);
			default: break ;
		}
}