#include <math.h>

#define WORLD_W 250
#define WORLD_H 250
#define world()

int world[WORLD_W * WORLD_H]; // first xcor, then ycor.
                    // 0 is floor, 1 is wall.


int collide(float, float);
