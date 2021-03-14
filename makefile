
CFLAGS = -Wall -std=c99

objects = main.o game-states.o lib-game-states.o player.o enemy.o utils.o \
			tad-lde.o screens.o

ALLEGRO = `pkg-config allegro-5 allegro_font-5 allegro_image-5 \
			allegro_primitives-5 allegro_audio-5 allegro_acodec-5 \
			allegro_image-5 --libs --cflags`


all: battle-city

battle-city: $(objects)
	gcc $(objects) -o battle-city $(ALLEGRO)

main.o: main.c
	gcc -c $(CFLAGS) main.c

game-states.o: game-states.c game-states.h
	gcc -c $(CFLAGS) game-states.c

lib-game-states.o: lib-game-states.c lib-game-states.h
	gcc -c $(CFLAGS) lib-game-states.c

player.o: player.c player.h 
	gcc -c $(CFLAGS) player.c

enemy.o: enemy.c enemy.h
	gcc -c $(CFLAGS) enemy.c

utils.o: utils.c utils.h
	gcc -c $(CFLAGS) utils.c

tad-lde.o: tad-lde.c tad-lde.h
	gcc -c $(CFLAGS) tad-lde.c

screens.o: screens.c screens.h
	gcc -c $(CFLAGS) screens.c

clean:
	rm -f *.o

purge: clean
	rm -f battle-city

debug: CFLAGS += -pg -g
debug: all