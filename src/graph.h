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
void scanline_texture_triangle_half(SDL_Surface*, int,int, int,int, int, int, double,double,double, SDL_Surface*, int, int, int, int, int, int);

// addtriangle


void draw();
void draw_texture(int**, SDL_Surface*);

extern void update_view();


// int in_screen(int,int,int,int,int,int);
int point_in_screen(int,int);

// to texture matrix
void add_wall(int, int, int, int, int, int);

// Based on code by Mikael Kalms
// http://www.lysator.liu.se/~mikaelk/doc/perspectivetexture/

struct TPolytri {
    float x1, y1, z1;
    float x2, y2, z2;
    float x3, y3, z3;
    float u1, v1;
    float u2, v2;
    float u3, v3;
    SDL_Surface *texture;
};

static float dizdx, duizdx, dvizdx, dizdy, duizdy, dvizdy;
static float xa, xb, iza, uiza, viza;
static float dxdya, dxdyb, dizdya, duizdya, dvizdya;
static SDL_Surface *texture;

static void drawtpolyperspsubtriseg(int y1, int y2);
void drawtpolyperspsubtri(struct TPolytri*);
