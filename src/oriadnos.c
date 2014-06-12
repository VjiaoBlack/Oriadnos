#include "oriadnos.h"

int main(int argc, char *argv[]) {

    is_initialized = 0;
    init();

    printf("q to quit\n");
    /* Initialise SDL Video */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    /* Open a 1280 x 800 screen */
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF );
    if (screen == NULL) {
        printf("Couldn't set screen mode to %d x %d: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    // testing
    SDL_WM_GrabInput( SDL_GRAB_ON );
    SDL_ShowCursor(0);

    xcor = 0;
    zcor = 0;
    ycor = 0;
    deg = 0;
    tilt = 0;

    int running = 1;
    mouse_x = 0;
    mouse_y = 0;
    mouse_rx = 0;
    mouse_ry = 0;

    setup_world();
    load_bmps();

    deg = 0;
    tilt = 0;
    while (running) {
        texturdmatrix = mat4_copy(texturematrix);
        dmatrix = mat4_copy(ematrix);
        get_input();
        respond_to_input();


        if (!is_initialized) {
            is_initialized = 1;
            deg = 0;
            tilt = 0;
            xcor = 0;
            ycor = 0;
            zcor = 0;

        }



        // update();

        // draws the scene
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        draw();

        SDL_Flip(screen);

        /* Sleep briefly to stop sucking up all the CPU time */
        SDL_Delay(1);
    }
    /* Exit the program */
    SDL_Quit();

    // dont forget to free the textures and stuff
    // when you use them, that is.

    exit(0);
    return 0;
}

void load_bmps() {
    wall = SDL_LoadBMP("res/wall.bmp"); // 894 by 894
    if (wall == NULL) {
        printf("help\n");
    }
}

void setup_world() {

    // draw box, bottom left front to top right back
//    // addtriangle(0,0,0,0,1,0,1,0,0);
    draw_box(-2,0,0,-1,2,-5,255,255,255);
    draw_box(0,0,0,1,2,-7,0,255,255);
    draw_box(-5,0,-7,1,2,-8,255,0,255);
    draw_box(-5,0,-4,-2,2,-5,255,255,0);
//    // draw_box(0,-5,0,5,0,5,255,255,0);
//    // add_wall(0,-5,0,5,0,0);

    // TODO: Create new method for drawing a rectangle with a texture?


    // x,y,z: right, up, towards you
    // draw_box(0,0,0,5,-5,1,255,255,255);
    // addtriangle(0,0,0,10,0,0,  10,0, .01,255,255,255); // long is x
    // addtriangle(0,0,5, 0,0,0, .01,0,   5,255,255,255); // short is z

    // draw_box(0,0,0,1,1,-1, 255,255,255);
    // teapot();
    // push(tmatrix);
    // tmatrix = identity();

    // addtriangle(-3,1,0,   -3,6,0,  2,1,0,   255,255,0);
    // addtriangle(-5,-1,-2, -2,4,-2, -1,0,-2, 0,255,255);
    // addtriangle(-7,2,2,   -1,3,2,  -3,-1,2, 255,0,255);

    // addtriangle(-3,0,0,  1,2,0,   2,1,0,    255,255,0);
    // addtriangle(-2,1,-2, 0,-2,-2, -1,-3,-2, 0,255,255);
    // addtriangle(-2,-2,-5, 1,3,5,   2,2,5,    255,0,255);

    scale(((double) D_W) / (S_W), ((double) D_H) / (S_H), ((double) D_W) / (S_W));
    transform();

    printf("texture edge:%d, texture dest:%d, edge matrix:%d\n",mat4_columns(texturematrix), mat4_columns(texturdmatrix),mat4_columns(ematrix));

    // tmatrix = pop();
}


void get_input() {
    SDL_Event event;

    mouse_rx = 0;
    mouse_ry = 0;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        }
        if (event.type == SDL_KEYUP) {  // any key is released
            keysHeld[event.key.keysym.sym] = 0;
        }
        if (event.type == SDL_KEYDOWN) {  // any key is pressed
            keysHeld[event.key.keysym.sym] = 1;
        }
        if (event.type == SDL_MOUSEMOTION) {
            mouse_x = event.motion.x;
            mouse_y = event.motion.y;

            mouse_rx = event.motion.xrel;
            mouse_ry = event.motion.yrel;
        }
    }
}

void update_view() {
    mat4_delete(tmatrix);
    tmatrix = identity();

    move(0-xcor, 0-ycor, zcor-1000);
    rotate('y', deg);
    rotate('x', tilt);
    move(xcor, ycor, 0-zcor+1000);

    move(0-xcor, 0 - ycor, zcor);

    transform_d();
}

void respond_to_input() {
    float rad = deg_to_rad(deg);

    if (keysHeld[SDLK_w]) {
        zcor += 5 * cos(rad);
        xcor += 5 * sin(rad);
        update_view();
    }
    if (keysHeld[SDLK_s]) {
        zcor -= 5 * cos(rad);
        xcor -= 5 * sin(rad);
        update_view();
    }
    if (keysHeld[SDLK_a]) {
        xcor += -5 * cos(rad);
        zcor -= -5 * sin(rad);
        update_view();
    }
    if (keysHeld[SDLK_d]) {
        xcor += 5 * cos(rad);
        zcor -= 5 * sin(rad);
        update_view();

    }
    if (keysHeld[SDLK_SPACE]) {
        if (keysHeld[SDLK_LSHIFT]) {
            ycor -= 5;
        update_view();

        } else {
            ycor += 5;
        update_view();

        }
    }
    if (keysHeld[SDLK_LEFT]) {
        deg -= 1;
            update_view();

    }

    if (keysHeld[SDLK_RIGHT]) {

        deg += 1;
        update_view();
    }

    if (keysHeld[SDLK_UP]) {
        tilt -= 1;
        update_view();
    }

    if (keysHeld[SDLK_DOWN]) {
        tilt += 1;
        update_view();
    }


    if (keysHeld[SDLK_q]) {
        exit(0);
    }


    deg += mouse_rx / 20;
    tilt += mouse_ry / 20;
}

void update() {

}
