
CFLAGS = -Wall -c



objects = main.o game-states.o lib-game-states.o player.o utils.o

ALLEGRO = `pkg-config allegro-5 allegro_font-5 allegro_image-5 \
			allegro_primitives-5 allegro_audio-5 allegro_acodec-5 \
			allegro_image-5 --libs --cflags`

all: battle-city

battle-city: $(objects)
	gcc $(objects) -o battle-city $(ALLEGRO)

main.o: main.c main.h
	gcc $(CFLAGS) main.c

game-states.o: game-states.c game-states.h
	gcc $(CFLAGS) game-states.c

lib-game-states.o: lib-game-states.c lib-game-states.h
	gcc $(CFLAGS) lib-game-states.c

player.o: player.c player.h 
	gcc $(CFLAGS) player.c

enemy.o: enemy.c enemy.h
	gcc $(CFLAGS) enemy.c

utils.o: utils.c utils.h
	gcc $(CFLAGS) utils.c

# da uma olhada no video explicando sobre debugs
debug:
	CFLAGS += -d

clean:
	rm -f *.o

purge: clean
	rm -f battle-city
