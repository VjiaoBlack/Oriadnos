COMPILE=gcc -g -Wall -O2 -c
LINK=gcc -g -Wall -O2
SDL_CFLAGS=`sdl-config --cflags`
SDL_LIBS=`sdl-config --libs`

all: oriadnos

clean:
	rm -rf oriadnos build/*.o

build:
	mkdir build

oriadnos: build build/oriadnos.o build/graph.o build/mat4.o build/trans.o build/collision.o
	$(LINK) build/oriadnos.o build/graph.o build/mat4.o build/trans.o build/collision.o -lm -o oriadnos $(SDL_LIBS)

build/oriadnos.o: src/oriadnos.c src/oriadnos.h
	$(COMPILE) src/oriadnos.c -o build/oriadnos.o $(SDL_CFLAGS)

build/graph.o: src/graph.c src/graph.h
	$(COMPILE) src/graph.c -o build/graph.o $(SDL_CFLAGS)

build/mat4.o: src/mat4.c src/mat4.h
	$(COMPILE) src/mat4.c -o build/mat4.o

build/trans.o: src/trans.c src/trans.h
	$(COMPILE) src/trans.c -o build/trans.o

build/collision.o: src/collision.c src/collision.h
	$(COMPILE) src/collision.c -o build/collision.o
