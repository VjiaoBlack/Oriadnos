#include <float.h>
#include "SDL.h"
#include "trans.h"

#define D_W 1200
#define D_H 800
#define S_W 24
#define S_H 16
#define EYE_X 12
#define EYE_Y 8
#define EYE_Z 20

SDL_Surface* screen;
SDL_Surface* wall;
double zbuf[D_H][D_W];

// to screen
void put_pixel(SDL_Surface*, int, int, Uint32);
void draw_line(SDL_Surface*, int,int,int,int, Uint8, Uint8, Uint8);
Uint32 get_pixel(SDL_Surface*, int, int);
void swap(int*,int*);

// to edge matrix
void draw_box(int,int,int, int,int,int, Uint8,Uint8,Uint8);
void half_scanline_triangle(SDL_Surface*, int, int, int, int, int, double, double, double, Uint8 , Uint8 , Uint8 );// to screen coordinates.
void scanline_triangle(SDL_Surface*, int,int,int,int,int,int,double,double,double,Uint8,Uint8,Uint8);

void scanline_texture(SDL_Surface*, int,int, int,int, int,int, int,int, double,double,double,double, SDL_Surface*);
void scanline_texture_triangle_half(SDL_Surface*, int,int, int,int, int, double,double,double, SDL_Surface*, int, int, int, int, int, int);

// addtriangle


void draw();
void draw_texture(int**, SDL_Surface*);

extern void update_view();


// int in_screen(int,int,int,int,int,int);
int point_in_screen(int,int);

// to texture matrix
void add_wall(int, int, int, int, int, int);
