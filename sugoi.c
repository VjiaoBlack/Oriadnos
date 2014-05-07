#include "sugoi.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 640


int main(int argc, char *argv[]) {

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
    xcor = 0;
    zcor = 0;
    ycor = 0;
    deg = 0;

    int running = 1;
    pixel_x = 100;
    pixel_y = 100;
    mouse_x = 0;
    mouse_y = 0;

    setup_world();

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

void setup_world() {
    // draw_box(-2,0,0,-1,2,-5,255,255,255);
    // draw_box(0,0,0,1,2,-7,255,255,255);
    // draw_box(-5,0,-7,1,2,-9,255,255,255);

    int inputargc;
    char** inputargv;
    float a1, a2, a3, b1, b2, b3, c1, c2, c3;

    FILE* fp = fopen("teapot.tri","r");
    char triangle[128];
    while (fgets(triangle,128,fp)) {
        inputargv = parse_split(triangle);
        inputargc = parse_numwords(inputargv);
        a1 = atof(inputargv[0]);
        a2 = atof(inputargv[1]);
        a3 = atof(inputargv[2]);
        b1 = atof(inputargv[3]);
        b2 = atof(inputargv[4]);
        b3 = atof(inputargv[5]);
        c1 = atof(inputargv[6]);
        c2 = atof(inputargv[7]);
        c3 = atof(inputargv[8]);
        addtriangle(a1,a2,a3,b1,b2,b3,c1,c2,c3);
    }

    printf("added all triangles\n");


    // push(tmatrix);
    // tmatrix = identity();
    scale(((double) D_W) / (S_W), ((double) D_H) / (S_H), ((double) D_W) / (S_W));
    transform();
    // tmatrix = pop();
}

void draw() {
    // SDL_LockSurface(screen);
    // // NOTE: you cannot put a pixel more than once.
    // put_pixel(screen,mouse_y,mouse_x,255,100,255);
    // put_pixel(screen,pixel_x,pixel_y,100,255,100);
    // SDL_UnlockSurface(screen);

    // draw_line(screen, 50,100,235,209,213,236,55);

    int ii = 0;

    while (ii < mat4_columns(ematrix)) {
        // print(ematrix);
        int rx, ry, rz, x1, y1, z1, x2, y2, z2, x3, y3, z3;
        double p1[3], p2[3], p3[3];

        rx = (int) ((EYE_X * D_W) / (S_W)  + D_W / 2);
        ry = (int) ((EYE_Y * D_H) / (S_H)  + D_H / 2);
        rz = (int) ((EYE_Z * D_W) / (S_W));


        x1 = rx - (int) mat4_get(ematrix, 0, ii);
        y1 = ry + (int) mat4_get(ematrix, 1, ii);
        z1 =            mat4_get(ematrix, 2, ii);
        x2 = rx - (int) mat4_get(ematrix, 0, ii+1);
        y2 = ry + (int) mat4_get(ematrix, 1, ii+1);
        z2 =            mat4_get(ematrix, 2, ii+1);
        x3 = rx - (int) mat4_get(ematrix, 0, ii+2);
        y3 = ry + (int) mat4_get(ematrix, 1, ii+2);
        z3 =            mat4_get(ematrix, 2, ii+2);

        // these are the points in 3d space
        p1[0] = mat4_get(ematrix, 0, ii);
        p1[1] = mat4_get(ematrix, 1, ii);
        p1[2] = mat4_get(ematrix, 2, ii);
        p2[0] = mat4_get(ematrix, 0, ii+1);
        p2[1] = mat4_get(ematrix, 1, ii+1);
        p2[2] = mat4_get(ematrix, 2, ii+1);
        p3[0] = mat4_get(ematrix, 0, ii+2);
        p3[1] = mat4_get(ematrix, 1, ii+2);
        p3[2] = mat4_get(ematrix, 2, ii+2); 

        x1 = rx - x1 * rz / (rz - z1) + D_W / 2;
        y1 = ry - y1 * rz / (rz - z1) + D_H / 2;
        x2 = rx - x2 * rz / (rz - z2) + D_W / 2;
        y2 = ry - y2 * rz / (rz - z2) + D_H / 2;
        x3 = rx - x3 * rz / (rz - z3) + D_W / 2;
        y3 = ry - y3 * rz / (rz - z3) + D_H / 2;
        // printf("drawing line %d, %d : %d, %d\n", x1, y1, x2, y2);

        // printf("testforvisible\n");
        if (isvisible(p1,p2,p3,rx,0-ry,rz,0)) {
            // printf("is visible\n");
            draw_line(screen, x1, y1, x2, y2,255,255,255);
            draw_line(screen, x3, y3, x2, y2,255,255,255);
            draw_line(screen, x3, y3, x1, y1,255,255,255);

        }

        ii += 3;

    }
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

void update_view() {
    mat4_delete(tmatrix);
    tmatrix = identity();
    
    move(xcor, ycor, zcor);
    rotate(deg, 'y');
    transform();
}

void respond_to_input() {
    if (keysHeld[SDLK_UP]) {
        zcor= 5;
        update_view();
    }
    if (keysHeld[SDLK_DOWN]) {
        zcor= -5;
        update_view();
    }
    if (keysHeld[SDLK_LEFT]) {
        xcor = 5;
        update_view();
    }
    if (keysHeld[SDLK_RIGHT]) {
        xcor = -5;
        update_view();

    }
    if (keysHeld[SDLK_SPACE]) {
        if (keysHeld[SDLK_LSHIFT]) {
            ycor = 5;
        update_view();

        } else {
            ycor = -5;
        update_view();

        }
    } 
    if (keysHeld[SDLK_a]) {
        deg = 5;
        update_view();

    }

    if (keysHeld[SDLK_d]) {
        deg = -5;
        update_view();

    }

    if (keysHeld[SDLK_q]) {
        exit(0);
    }
}

void update() {

}
