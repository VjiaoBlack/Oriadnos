#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define WORLD_W 50
#define WORLD_H 50

#define COLLIDES(x, z) test_collision((x) * ((double) S_W) / D_W, ((z) - Z_OFF) * ((double) S_W) / D_W)

bool collision_map[WORLD_H][WORLD_W];

bool test_collision(int, int);
void print_collision_map(int, int);
