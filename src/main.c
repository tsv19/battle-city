
#include "utils.h"
#include "game-states.h"

// lib-game-states variable
STATE_GAME state;

int main ()
{
	state = START;
	// loop principal
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