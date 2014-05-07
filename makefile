all: sugoi

sugoi: sugoi.o graph.o
	gcc -g -Wall sugoi.o graph.o -lm -o sugoi `sdl-config --cflags --libs`

sugoi.o: sugoi.c sugoi.h 
	gcc -g -Wall -c sugoi.c -o sugoi.o `sdl-config --cflags --libs`

graph.o: graph.c graph.h
	gcc -g -Wall -c graph.c -o graph.o `sdl-config --cflags --libs`
