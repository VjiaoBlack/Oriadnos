#include "SDL.h"
#include "trans.h"

#define D_W 800
#define D_H 640
#define S_W 16
#define S_H 12.8

// to screen
void put_pixel(SDL_Surface*, int, int, Uint8, Uint8, Uint8); 
void draw_line(SDL_Surface*, int,int,int,int,Uint8,Uint8,Uint8); 
void swap(int*,int*);

// to edge matrix
void draw_box(int,int,int, int,int,int, Uint8,Uint8,Uint8);
