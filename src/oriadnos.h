#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "graph.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

// one input unit = 50 pixels

int is_initialized;

float xcor;
float zcor;
float ycor;

int deg;
int tilt;

double render_time = 0;

Mat4** namedtdict;
int namedsize;
char** storagenames;


int keysHeld[323] = {0};

int mouse_x;
int mouse_y;
int mouse_rx;
int mouse_ry;

void get_input();
void respond_to_input();
void setup_world();
void update_view();
void load_bmps();
