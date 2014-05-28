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
        printf("Couldn't set screen mode to 640 x 480: %s\n", SDL_GetError());
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

        dmatrix = mat4_copy(ematrix);
        get_input();
        respond_to_input();

        printf("x:%f, y:%f, z:%f, deg:%d, tilt:%d\n", xcor, ycor, zcor, deg, tilt);

        if (!is_initialized) {
            is_initialized = 1;
            deg = 0;
            tilt = 0;
            xcor = 0;
            ycor = 0;
            zcor = 0;

        }

        printf("test\n");


        // update();

        // draws the scene
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        draw();
        printf("test\n");

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
    wall = SDL_LoadBMP("wall.bmp"); // 894 by 894
}

void setup_world() {

    // draw box, bottom left front to top right back
    // addtriangle(0,0,0,0,1,0,1,0,0);
    // draw_box(-2,0,0,-1,2,-5,255,255,255);
    // draw_box(0,0,0,1,2,-7,0,255,255);
    // draw_box(-5,0,-7,1,2,-8,255,0,255);
    // draw_box(-5,0,-4,-2,2,-5,255,255,0);


    // x,y,z: right, up, towards you
    // draw_box(0,0,0,5,-5,1,255,255,255);
    addtriangle(0,0,0,10,0,0,  10,0, .01,255,255,255); // long is x
    addtriangle(0,0,5, 0,0,0, .01,0,   5,255,255,255); // short is z

    draw_box(0,0,0,1,1,-1, 255,255,255);
    // teapot();
    // push(tmatrix);
    // tmatrix = identity();


    scale(((double) D_W) / (S_W), ((double) D_H) / (S_H), ((double) D_W) / (S_W));
    transform();


    // tmatrix = pop();
}

void draw() {
    // SDL_LockSurface(screen);
    // // NOTE: you cannot put a pixel more than once.
    // SDL_UnlockSurface(screen);

    // draw_line(screen, 50,100,235,209,213,236,55);

    // TEST TO GET TEXTURE

    // print(dmatrix);


    // END TEST


    int ii = 0;
    update_view();

    int* screenverticies[2];
    screenverticies[0] = malloc(sizeof(int) * mat4_columns(dmatrix));
    screenverticies[1] = malloc(sizeof(int) * mat4_columns(dmatrix));
        printf("test\n");

    while (ii < mat4_columns(dmatrix)) {
        // print(dmatrix);
        printf("test1\n");


        screenverticies[0][ii] = -1;
        screenverticies[1][ii] = -1;
        screenverticies[0][ii+1] = -1;
        screenverticies[1][ii+1] = -1;
        screenverticies[0][ii+2] = -1;
        screenverticies[1][ii+2] = -1;

        int rx, ry, rz, x1, y1, z1, x2, y2, z2, x3, y3, z3;
        int a = 0, b = 0, c = 0;
        double p1[3], p2[3], p3[3];

        rx = (int) (((0 - EYE_X) * D_W) / (S_W)  + D_W / 2);
        ry = (int) (((0 - EYE_Y) * D_H) / (S_H)  + D_H / 2);
        rz = (int) ((EYE_Z * D_W) / (S_W));


        x1 = rx - (int) mat4_get(dmatrix, 0, ii);
        y1 = ry + (int) mat4_get(dmatrix, 1, ii);
        z1 =            mat4_get(dmatrix, 2, ii);
        x2 = rx - (int) mat4_get(dmatrix, 0, ii+1);
        y2 = ry + (int) mat4_get(dmatrix, 1, ii+1);
        z2 =            mat4_get(dmatrix, 2, ii+1);
        x3 = rx - (int) mat4_get(dmatrix, 0, ii+2);
        y3 = ry + (int) mat4_get(dmatrix, 1, ii+2);
        z3 =            mat4_get(dmatrix, 2, ii+2);

        if (z1 < 1000) {
            a = 1;
        }
        if (z2 < 1000) {
            b = 1;
        }
        if (z3 < 1000) {
            c = 1;
        }

        // these are the points in 3d space
        if (abs(rz - z1) > 1) {
            p1[0] = mat4_get(dmatrix, 0, ii);
            p1[1] = mat4_get(dmatrix, 1, ii);
            p1[2] = mat4_get(dmatrix, 2, ii);
            x1 = rx - x1 * rz / (rz - z1) + D_W / 2;
            y1 = ry - y1 * rz / (rz - z1) + D_H / 2;
            screenverticies[0][ii] = x1;
            screenverticies[1][ii] = y1;
        }
        if (abs(rz - z2) > 1) {
            p2[0] = mat4_get(dmatrix, 0, ii+1);
            p2[1] = mat4_get(dmatrix, 1, ii+1);
            p2[2] = mat4_get(dmatrix, 2, ii+1);
            x2 = rx - x2 * rz / (rz - z2) + D_W / 2;
            y2 = ry - y2 * rz / (rz - z2) + D_H / 2;
            screenverticies[0][ii+1] = x2;
            screenverticies[1][ii+1] = y2;
        }
        if (abs(rz - z3) > 1) {
            p3[0] = mat4_get(dmatrix, 0, ii+2);
            p3[1] = mat4_get(dmatrix, 1, ii+2);
            p3[2] = mat4_get(dmatrix, 2, ii+2);
            x3 = rx - x3 * rz / (rz - z3) + D_W / 2;
            y3 = ry - y3 * rz / (rz - z3) + D_H / 2;
            screenverticies[0][ii+2] = x3;
            screenverticies[1][ii+2] = y3;
        }

        printf("%d, %d, %d, %d, %d, %d\n", x1, y1, x2, y2, x3, y3);
        if (isvisible   (p1,p2,p3,rx,0-ry,rz,0)) {
            printf("test2\n");
            scanline_triangle(screen, x1,y1,x2,y2,x3,y3, mat4_get(cmatrix,0,ii), mat4_get(cmatrix,1,ii), mat4_get(cmatrix,2,ii));
            printf("gud\n");
        // printf("is visible\n");
            if (a && b) {
                draw_line(screen, x1, y1, x2, y2, mat4_get(cmatrix,0,ii),mat4_get(cmatrix,1,ii),mat4_get(cmatrix,2,ii));

            }
            if (b && c) {
                draw_line(screen, x3, y3, x2, y2, mat4_get(cmatrix,0,ii + 1),mat4_get(cmatrix,1,ii + 1),mat4_get(cmatrix,2,ii+ 1));

            }
            if (a && c) {
                draw_line(screen, x3, y3, x1, y1, mat4_get(cmatrix,0,ii + 2),mat4_get(cmatrix,1,ii + 2),mat4_get(cmatrix,2,ii+ 2));

            }
        }

        ii += 3;


    }

    // draw_texture(screenverticies, wall);
    fill_rectangle(screenverticies, 100,50,25);
}

void fill_rectangle(int** screenverticies, Uint8 r, Uint8 b, Uint8 g) { //

    // hope:
    // bresenham left and right simultanously,
    // then just bresenham through from the left to right.

    float brx = screenverticies[0][2];
    float bry = screenverticies[1][2];
    float try = screenverticies[1][1];
    float trx = screenverticies[0][1];
    float blx = screenverticies[0][0];
    float bly = screenverticies[1][0];
    float tly = screenverticies[1][4];
    float tlx = screenverticies[0][4];

    // float x1i = tlx;
    // float y1i = tly;
    // float x2i = trx;
    // float y2i = try;
    // float x1f = blx;
    // float y1f = bly;
    // float x2f = brx;
    // float y2f = bry;


    float dx1 = (float) (tlx - blx) / (float) 800;
    float dy1 = (float) (tly - bly) / (float) 800;
    float dx2 = (float) (trx - brx) / (float) 800;
    float dy2 = (float) (try - bry) / (float) 800;



    float xc = 0;
    float yc = 0;

    // scanline conversion;
}

void draw_texture(int** screenverticies, SDL_Surface* surface) {
    // col 1 is bottom left
    // col 3 is bottom right
    // col 2 is top right
    // col 5 is top left

    int brx = screenverticies[0][2];
    int bry = screenverticies[1][2];

    int try = screenverticies[1][1];
    int trx = screenverticies[0][1];

    int blx = screenverticies[0][0];
    int bly = screenverticies[1][0];

    int tly = screenverticies[1][4]; // 4
    int tlx = screenverticies[0][4]; // not 0, 1, 2, 3

    // printf("%d,%d %d,%d %d,%d %d,%d\n", tlx, tly, blx, bly, trx, try, brx, bry);


    // float dxl = (tlx - blx) / 500; // increment per vertex from top, left edge
    // float dyl = (tly - bly) / 500;

    // float dxr = (trx - brx) / 500; // increment per vertex from top, right edge
    // float dyr = (try - bry) / 500;

    float x1 = tlx;
    float y1 = tly;
    float x2 = trx;
    float y2 = try;

    // float dx1 = (float) (tlx - blx) / (float) 800;
    // float dy1 = (float) (tly - bly) / (float) 800;
    // float dx2 = (float) (trx - brx) / (float) 800;
    // float dy2 = (float) (try - bry) / (float) 800;

    float xc = 0;
    float yc = 0;

    int x = 0;
    int y = 0;
    for (y = 0; y < 800; y++) {
        for (x = 0; x < 800; x++) { // goes through image row by row
            yc = (y2 - y1) * ((float)x / (float)800) + y1;
            xc = (x2 - x1) * ((float)x / (float)800) + x1;
            if (xc > 0 && xc < 1200 && yc > 0 && yc < 800) {
                put_pixel(screen, xc,    yc, get_pixel(surface,x,y));
            }
        }
        x1 = 0 - (float) y * (float) (tlx - blx) / (float) 800 + tlx;
        y1 = 0 - (float) y * (float) (tly - bly) / (float) 800 + tly;
        x2 = 0 - (float) y * (float) (trx - brx) / (float) 800 + trx;
        y2 = 0 - (float) y * (float) (try - bry) / (float) 800 + try;
    }
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
    // move(0-xcor, 0-ycor, 0-zcor);
    // move(xcor,  ycor, zcor+1000);
    move(0-xcor, 0-ycor, zcor-1000);
    rotate('y', deg);
    rotate('x', tilt);
    move(xcor, ycor, 0-zcor+1000);

    move(0-xcor, 0 - ycor, zcor);
    transform_d();

    // printf("%f, %f, %f, %d\n", xcor, ycor, zcor + 1000, deg);
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
