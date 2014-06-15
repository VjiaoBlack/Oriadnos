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
#define Z_OFF (((double) (EYE_Z * D_W)) / (S_W))

SDL_Surface* screen;
SDL_Surface* wall;
double zbuf[D_H][D_W];

// to screen
void put_pixel(SDL_Surface*, int, int, Uint32);
Uint32 get_pixel(SDL_Surface*, int, int);

// to edge matrix
void draw_box(int,int,int, int,int,int, Uint8,Uint8,Uint8);

void draw();
void draw_texture(int**, SDL_Surface*);

extern void update_view();

inline int point_in_screen(int,int);

void scanline_texture(SDL_Surface*, double, double, double, double, double,
    double, double, double, double, double, double, double, double, double, double);
void scanline_texture_segment(SDL_Surface*, int, int);

// to texture matrix
void add_wall(int, int, int, int, int, int);

// shared variables between scanline_texture() and scanline_texture_segment()
double dizdx, duizdx, dvizdx, dizdy, duizdy, dvizdy;
double xa, xb, iza, uiza, viza;
double dxdya, dxdyb, dizdya, duizdya, dvizdya;
