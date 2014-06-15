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

    setup_world();
    scale(((double) D_W) / S_W, ((double) D_H) / S_H, ((double) D_W) / S_W);
    transform();
    load_bmps();

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
    double frame_seconds;
    long long frame = 0;
    int i;
    for (i = 0; i < FPS_SAMPLES; i++)
        FPS_DATA[i] = MAX_FPS;

    while (running) {
        gettimeofday(&start, NULL);

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
        snprintf(fps_info, 15, "FPS: %d", get_fps());

        // draws the scene
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        draw();
        font_surface = TTF_RenderText_Blended(font, fps_info, font_color);
        SDL_BlitSurface(font_surface, NULL, screen, NULL);
        SDL_FreeSurface(font_surface);
        SDL_Flip(screen);

        gettimeofday(&end, NULL);
        frame_seconds = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
        if (frame_seconds < 1. / MAX_FPS) {
            FPS_DATA[(frame++) % FPS_SAMPLES] = MAX_FPS;
            SDL_Delay(1000 * (1. / MAX_FPS - frame_seconds));
        }
        else
            FPS_DATA[(frame++) % FPS_SAMPLES] = 1 / frame_seconds;
    }

    SDL_Quit();
    TTF_CloseFont(font);
    SDL_FreeSurface(wall);
    SDL_FreeSurface(screen);
    exit(0);
    return 0;
}

int get_fps() {
    int sum = 0, i;
    for (i = 0; i < FPS_SAMPLES; i++)
        sum += FPS_DATA[i];
    return sum / FPS_SAMPLES;
}

void load_bmps() {
    SDL_Surface* wall_temp = SDL_LoadBMP("res/wall.bmp");
    if (!wall_temp) {
        printf("loading wall BMP failed\n");
        exit(1);
    }
    wall = SDL_ConvertSurface(wall_temp, screen->format, 0);
    SDL_FreeSurface(wall_temp);
}

void setup_world() {
    add_wall(-1,1,-3,  1,-1,-3);
    add_wall(-3,1,-3, -1,-1,-3);
    add_wall(-5,1,-3, -3,-1,-3);

    add_wall(-3,1,4, -1,-1,4);
    add_wall(-1,1,4, -1,-1,2);
    add_wall(-1,1,2, -1,-1,0);
    add_wall(-1,1,0, -1,-1,-1);

    add_wall(1,1,4,  3,-1,4);
    add_wall(1,1,2,  1,-1,4);
    add_wall(1,1,0,  1,-1,2);
    add_wall(1,1,-2, 1,-1,0);
    add_wall(1,1,-3, 1,-1,-2);

    add_wall(-1,1,-1, -3,-1,-1);
    add_wall(-3,1,-1, -5,-1,-1);
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

    move(0-xcor, 0-ycor, zcor - 1000);
    rotate('y', deg);
    rotate('x', tilt);
    move(xcor, ycor, 0-zcor + 1000);

    move(0 - xcor, 0 - ycor, zcor);

    transform_d();
}

void respond_to_input() {
    float rad = deg_to_rad(deg);
    int relative_frames = (MAX_FPS / get_fps()) + 0.5;

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
        if (keysHeld[SDLK_LSHIFT])
            ycor -= 5;
        else
            ycor += 5;
        update_view();
    }
    if (keysHeld[SDLK_LEFT]) {
        deg -= relative_frames;
        update_view();
    }
    if (keysHeld[SDLK_RIGHT]) {
        deg += relative_frames;
        update_view();
    }
    if (keysHeld[SDLK_UP]) {
        tilt -= relative_frames;
        update_view();
    }
    if (keysHeld[SDLK_DOWN]) {
        tilt += relative_frames;
        update_view();
    }
    if (keysHeld[SDLK_q]) {
        exit(0);
    }

    deg += mouse_rx / 20;
    tilt += mouse_ry / 20;
}
