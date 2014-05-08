#include "SDL.h"
#include "trans.h"

#define D_W 1200
#define D_H 800
#define S_W 24
#define S_H 16

// to screen
void put_pixel(SDL_Surface*, int, int, Uint8, Uint8, Uint8); 
void draw_line(SDL_Surface*, int,int,int,int,Uint8,Uint8,Uint8); 
void swap(int*,int*);

// to edge matrix
void draw_box(int,int,int, int,int,int, Uint8,Uint8,Uint8);
// addtriangle
