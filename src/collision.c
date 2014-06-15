#include "collision.h"

bool test_collision(int xcor, int zcor) {
    return collision_map[zcor + WORLD_H / 2][xcor + WORLD_W / 2];
}

void print_collision_map(int xcor, int zcor) {
    int x, z;
    printf("+ ");
    for (x = 0; x < WORLD_W; x++)
        printf("- ");
    printf("+\n");
    for (z = WORLD_H - 1; z >= 0; z--) {
        printf("| ");
        for (x = 0; x < WORLD_W; x++) {
            if (x == xcor + WORLD_W / 2 && z == zcor + WORLD_H / 2)
                printf("* ");
            else
                printf("%c ", collision_map[z][x] ? '#' : '.');
        }
        printf("|\n");
    }
    printf("+ ");
    for (x = 0; x < WORLD_W; x++)
        printf("- ");
    printf("+\n");
}
