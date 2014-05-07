all: sugoi

sugoi: sugoi.o graph.o mat4.o trans.o
	gcc -g -Wall sugoi.o graph.o mat4.o trans.o -lm -o sugoi `sdl-config --cflags --libs`

sugoi.o: sugoi.c sugoi.h  
	gcc -g -Wall -c sugoi.c -o sugoi.o `sdl-config --cflags --libs`

graph.o: graph.c graph.h 
	gcc -g -Wall -c graph.c -o graph.o `sdl-config --cflags --libs`

mat4.o: mat4.c mat4.h
	gcc -g -Wall -c mat4.c -o mat4.o

trans.o: trans.c trans.h
	gcc -g -Wall -c trans.c -o trans.o
