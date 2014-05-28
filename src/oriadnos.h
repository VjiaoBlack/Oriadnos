#include "SDL.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

// one input unit = 50 pixels

#define EYE_X 12
#define EYE_Y 8
#define EYE_Z 20

int is_initialized;

float xcor;
float zcor;
float ycor;

int deg;
int tilt;


Mat4** namedtdict;
int namedsize;
char** storagenames;

SDL_Surface* screen;
SDL_Surface* wall;

int keysHeld[323] = {0};

int mouse_x;
int mouse_y;
int mouse_rx;
int mouse_ry;

void get_input();
void update();
void respond_to_input();
void setup_world();
void update_view();
void load_bmps();
