#include "SDL.h"
#include <stdio.h>

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
