#include "sugoi.h"
#include "graph.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 640


int main(int argc, char *argv[]) {
    printf("q to quit\n");
    /* Initialise SDL Video */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    /* Open a 1280 x 800 screen */
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF );
    if (screen == NULL) {
        printf("Couldn't set screen mode to 640 x 480: %s\n", SDL_GetError());
        exit(1);
    }

    int running = 1;
    pixel_x = 100;
    pixel_y = 100;
    mouse_x = 0;
    mouse_y = 0;
    while (running) {
        get_input();  
        respond_to_input();

        // update();

        // draws the scene
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        draw();
        SDL_Flip(screen);

        /* Sleep briefly to stop sucking up all the CPU time */
        SDL_Delay(16);
    }
    /* Exit the program */    
    SDL_Quit();

    // dont forget to free the textures and stuff
    // when you use them, that is.

    exit(0);
    return 0;
}

void draw() {
    SDL_LockSurface(screen);
    Uint32 pixel1;
    Uint32 pixel2;
    pixel1 = SDL_MapRGB(screen->format, 0xff,0x00,0xff);
    pixel2 = SDL_MapRGB(screen->format, 0x00,0xff,0xff);
    // NOTE: you cannot put a pixel more than once.
    put_pixel(screen,mouse_y,mouse_x,pixel1);
    put_pixel(screen,pixel_x,pixel_y,pixel2);
    SDL_UnlockSurface(screen);
}

void get_input() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_Quit) {
            exit(0);
        }
        if (event.type == SDL_KEYUP) { // any key is released
            keysHeld[event.key.keysym.sym] = 0;
        }
        if (event.type == SDL_KEYDOWN) { // any key is pressed
            keysHeld[event.key.keysym.sym] = 1;
        } 
        if (event.type == SDL_MOUSEMOTION) {
            mouse_x = event.motion.x;
            mouse_y = event.motion.y;
        }
        
    }
}

void respond_to_input() {
    if (keysHeld[SDLK_UP]) {
        pixel_y--;
    }
    if (keysHeld[SDLK_DOWN]) {
        pixel_y++;
    }
    if (keysHeld[SDLK_LEFT]) {
        pixel_x--;
    }
    if (keysHeld[SDLK_RIGHT]) {
        pixel_x++;
    }
    if (keysHeld[SDLK_q]) {
        exit(0);
    }
}

void update() {

}
