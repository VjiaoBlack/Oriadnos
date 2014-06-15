#include "oriadnos.h"

int main(int argc, char *argv[]) {

    is_initialized = 0;
    init();

    setup_world();
    load_bmps();

    printf("q to quit\n");
    /* Initialise SDL Video */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    if (TTF_Init() < 0) {
        printf("Could not initialize SDL_ttf: %s\n", TTF_GetError());
        exit(1);
    }

    /* Open a screen */
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (screen == NULL) {
        printf("Couldn't set screen mode to %d x %d: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_ShowCursor(0);

    TTF_Font* font = TTF_OpenFont("res/OpenSans.ttf", 24);;
    if (!font) {
        printf("Couldn't load font: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }
    SDL_Color font_color = {255, 255, 255};
    SDL_Surface *font_surface;

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

    struct timeval start, end;

    while (running) {
        gettimeofday(&start, NULL);

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

        char fps_info[15];
        snprintf(fps_info, 15, "FPS: %0.3f", 1 / render_time);

        // draws the scene
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        draw();
        font_surface = TTF_RenderText_Blended(font, fps_info, font_color);
        SDL_BlitSurface(font_surface, NULL, screen, NULL);
        SDL_FreeSurface(font_surface);
        SDL_Flip(screen);

        gettimeofday(&end, NULL);
        render_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
        if (render_time < 1. / 60)
            SDL_Delay(1. / 60 - render_time);
    }

    SDL_Quit();

    // dont forget to free the textures and stuff
    // when you use them, that is.

    exit(0);
    return 0;
}

void load_bmps() {
    wall = SDL_LoadBMP("res/test.bmp"); // 500 by 500
    if (wall == NULL) {
        printf("help\n");
    }
}

void setup_world() {
    // draw box, bottom left front to top right back

    // Z-buffering test triangle set A:
    // addtriangle(-3,1,0,   -3,6,0,  2,1,0,   255,255,0);
    // addtriangle(-5,-1,-2, -2,4,-2, -1,0,-2, 0,255,255);
    // addtriangle(-7,2,2,   -1,3,2,  -3,-1,2, 255,0,255);

    // Z-buffering test triangle set B:
    // addtriangle(-3,0,0,  1,2,0,   2,1,0,    255,255,0);
    // addtriangle(-2,1,-2, 0,-2,-2, -1,-3,-2, 0,255,255);
    // addtriangle(-2,-2,-5, 1,3,5,   2,2,5,    255,0,255);

    // Basic texture test:
    draw_box(0,5,0,5,0,-5,255,255,0);
    add_wall(0,5,0,5,0,0);

    // Basic maze test:
    // draw_box(-2,0,0,-1,2,-5,   255,255,255);
    // draw_box(0,0,0,1,2,-7,     0,255,255);
    // draw_box(-5,0,-7,1,2,-8,   255,0,255);
    // draw_box(-5,0,-4,-2,2,-5,  255,255,0);

    scale(((double) D_W) / (S_W), ((double) D_H) / (S_H), ((double) D_W) / (S_W));
    transform();

    // _TEST_
    // printf("texture edge:%d, texture dest:%d, edge matrix:%d\n",mat4_columns(texturematrix), mat4_columns(texturdmatrix),mat4_columns(ematrix));

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
