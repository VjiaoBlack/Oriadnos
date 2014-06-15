#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "graph.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

#define MAX_FPS 60
#define FPS_SAMPLES 10

// one input unit = 50 pixels

int is_initialized;

float xcor;
float zcor;
float ycor;

int deg;
int tilt;

Mat4** namedtdict;
int namedsize;
char** storagenames;

int FPS_DATA[FPS_SAMPLES];

int keysHeld[323] = {0};

int mouse_x;
int mouse_y;
int mouse_rx;
int mouse_ry;

int get_fps();
void get_input();
void respond_to_input();
void setup_world();
void update_view();
void load_bmps();
