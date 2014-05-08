#include "SDL.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "parse_util.h"

#define EYE_X -12
#define EYE_Y -9
#define EYE_Z 20

float xcor;
float zcor;
float ycor;

int deg;


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
void update_view();
