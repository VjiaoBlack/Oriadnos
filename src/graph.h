#include <float.h>
#include "SDL.h"
#include "trans.h"
#include "collision.h"

#define D_W 1200
#define D_H 800
#define S_W 24
#define S_H 16
#define EYE_X 12
#define EYE_Y 8
#define EYE_Z 20

#define X_OFF (((float) -EYE_X * D_W) / (S_W) + D_W / 2)
#define Y_OFF (((float) -EYE_Y * D_H) / (S_H) + D_H / 2)
#define Z_OFF (((float) (EYE_Z * D_W)) / (S_W))

#define Z_FAR 750
#define ENABLE_SHADING 1

typedef struct {
   Uint8 r;
   Uint8 g;
   Uint8 b;
} pixel_t;

typedef struct {
    int width;
    int height;
    pixel_t* pixels;
} image_t;

float flicker;
int inc;

SDL_Surface* screen;
image_t wall;
image_t flor;
float zbuf[D_H][D_W];

// to screen
Uint32 get_pixel(image_t*, int, int);
void put_pixel(int, int, Uint32);

// to edge matrix
void draw_box(int,int,int, int,int,int, Uint8,Uint8,Uint8);

void draw();

extern void update_view();

void scanline_texture(image_t*, float, float, float, float, float,
    float, float, float, float, float, float, float, float, float, float);
void scanline_texture_segment(image_t*, int, int);

// to texture matrix
void add_wall(int, int, int, int);
void add_floor(int, int, int, int, int);

// shared variables between scanline_texture() and scanline_texture_segment()
float dizdx, duizdx, dvizdx, dizdy, duizdy, dvizdy;
float xa, xb, iza, uiza, viza;
float dxdya, dxdyb, dizdya, duizdya, dvizdya;
