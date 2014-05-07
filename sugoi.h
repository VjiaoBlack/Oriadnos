#include "SDL.h"
#include "graph.h"
#include <stdio.h>

#define EYE_X -8
#define EYE_Y -8
#define EYE_Z 20

int xcor;
int zcor;


Mat4** namedtdict;
int namedsize;
char** storagenames;

SDL_Surface* screen;
int keysHeld[323] = {0};
int mouse_x;
int mouse_y;
int pixel_x;
int pixel_y;

void draw();
void get_input();
void update();
void respond_to_input();
void setup_world();
