COMPILE=gcc -g -Wall -c
BUILD=gcc -g -Wall
SDL_LIBS=`sdl-config --cflags --libs`

all: oriadnos

clean:
	rm -rf oriadnos bin/*.o

bin:
	mkdir bin

oriadnos: bin bin/oriadnos.o bin/graph.o bin/mat4.o bin/trans.o bin/collision.o
	$(BUILD) bin/oriadnos.o bin/graph.o bin/mat4.o bin/trans.o bin/collision.o -lm -o oriadnos $(SDL_LIBS)

bin/oriadnos.o: src/oriadnos.c src/oriadnos.h
	$(COMPILE) src/oriadnos.c -o bin/oriadnos.o $(SDL_LIBS)

bin/graph.o: src/graph.c src/graph.h
	$(COMPILE) src/graph.c -o bin/graph.o $(SDL_LIBS)

bin/mat4.o: src/mat4.c src/mat4.h
	$(COMPILE) src/mat4.c -o bin/mat4.o

bin/trans.o: src/trans.c src/trans.h
	$(COMPILE) src/trans.c -o bin/trans.o

bin/collision.o: src/collision.c src/collision.h
	$(COMPILE) src/collision.c -o bin/collision.o
