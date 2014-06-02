#include "graph.h"


Uint32 get_pixel(SDL_Surface *surface, int x, int y) {

    // assumes bytes per pixel = 3
    int bpp = 3;

    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp + 1;

    if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
        return p[0] << 16 | p[1] << 8 | p[2];  // 0 is red, 8 is green, 16 is blue
    } else {
        return p[0] << 8 | p[1] << 0 | p[2] << 16;    // 1 is red, 2 is blue, 0 is green
    }

}

void put_pixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {

    // assumes bytes per pixel = 4
    int bpp = 4;
    if (y <= 0) {
        printf("outofrange\n");
    }
    Uint8 *p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp + 1;
    *(Uint32*)p = pixel;

}

void draw_line(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b) {
    int acc = 0,
        delta = 0,
        x, y, up, right;

    Uint32 pixel;

        if (x1 == x2 && y1 == y2) {
            if (x1 > 0 && y1 > 0 && x1 < D_W && y1 < D_H) {
                pixel = SDL_MapRGB(surface->format, r,g,b);

                put_pixel(surface, x1,y1,pixel);
            }
        } else if (abs(x1 - x2) >= abs(y1 - y2)) {
            if (x1 > x2) {
                swap(&x1,&x2);
                swap(&y1,&y2);
            }
            delta = abs(x2 - x1);
            acc += delta / 2;
            x = x1;
            y = y1;
            up = (y2 < y1);

            while (x <= x2) {
                if (x > 0 && y > 0 && x < D_W && y < D_H) {
                    pixel = SDL_MapRGB(surface->format, r,g,b);
                    put_pixel(surface, x,y,pixel);
                }
                acc += abs(y2 - y1);
                if (acc >= delta) {
                    acc -= delta;
                    if (up)
                        y--;
                    else
                        y++;
                }
                x++;
            }
        } else if (abs(x1 - x2) < abs(y1 - y2)) {
            if (y1 > y2) {
                swap(&x1,&x2);
                swap(&y1,&y2);
            }
            delta = abs(y2 - y1);
            acc += delta / 2;
            x = x1;
            y = y1;
            right = (x2 > x1);
            while (y <= y2) {
                if (x > 0 && y > 0 && x < D_W && y < D_H) {
                    pixel = SDL_MapRGB(surface->format, r,g,b);
                    put_pixel(surface,x,y,pixel);
                }
                acc += abs(x2 - x1);
                if (acc >= delta) {
                    acc -= delta;
                    if (right)
                        x++;
                    else
                        x--;
                }
                y++;
            }


        x = y = 0;
    }
}

void swap(int* a, int* b) {
    int c = *a;
    *a = *b;
    *b = c;
}

void draw_box(int l, int b, int f, int r, int t, int k, Uint8 cr, Uint8 cg, Uint8 cb) {
    // p1 is the bottom left close to you vertex, p2 is the top right far away vertex.

    // facing you
    addtriangle(l,t,f, r,t,f, l,b,f, cr, cg, cb);// ,r,g,b);
    addtriangle(l,b,f, r,t,f, r,b,f, cr, cg, cb);// ,r,g,b);
    // to the right
    addtriangle(r,t,f, r,t,k, r,b,f, cr, cg, cb);// ,r,g,b);
    addtriangle(r,b,f, r,t,k, r,b,k, cr, cg, cb);// ,r,g,b);
    // away from you
    addtriangle(r,t,k, l,t,k, r,b,k, cr, cg, cb);// ,r,g,b);
    addtriangle(r,b,k, l,t,k, l,b,k, cr, cg, cb);// ,r,g,b);
    // to the left
    addtriangle(l,t,k, l,t,f, l,b,k, cr, cg, cb);// ,r,g,b);
    addtriangle(l,b,k, l,t,f, l,b,f, cr, cg, cb);// ,r,g,b);
    // top
    addtriangle(l,t,k, r,t,k, l,t,f, cr, cg, cb);// ,r,g,b);
    addtriangle(l,t,f, r,t,k, r,t,f, cr, cg, cb);// ,r,g,b);
    // bottom
    addtriangle(l,b,f, r,b,f, l,b,k, cr, cg, cb);// ,r,g,b);
    addtriangle(l,b,k, r,b,f, r,b,k, cr, cg, cb);// ,r,g,b);
}

void organize(int x1, int y1, int x2, int y2, int x3, int y3, int* tx, int* ty, int* mx, int* my, int* bx, int* by) {
    if (y1 > y2) {
        if (y2 > y3) {
            *tx = x1;
            *ty = y1;
            *mx = x2;
            *my = y2;
            *bx = x3;
            *by = y3;
        } else {
            if (y1 > y3) {
                *tx = x1;
                *ty = y1;
                *mx = x3;
                *my = y3;
                *bx = x2;
                *by = y2;
            } else {
                *tx = x3;
                *ty = y3;
                *mx = x1;
                *my = y1;
                *bx = x2;
                *by = y2;
            }
        }
    } else {
        if (y1 > y3) {
            *tx = x2;
            *ty = y2;
            *mx = x1;
            *my = y1;
            *bx = x3;
            *by = y3;
        } else {
            if (y2 > y3) {
                *tx = x2;
                *ty = y2;
                *mx = x3;
                *my = y3;
                *bx = x1;
                *by = y1;
            } else {
                *tx = x3;
                *ty = y3;
                *mx = x2;
                *my = y2;
                *bx = x1;
                *by = y1;
            }
        }
    }
}

void scanline_triangle(SDL_Surface* surface, int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r, Uint8 g, Uint8 b) { // to screen coordinates.
    int tx, ty, mx, my, bx, by; // top, middle, bottom

    int increment;

    Uint32 pixel;

    organize(x1,y1,x2,y2,x3,y3,&tx,&ty,&mx,&my,&bx,&by);

    // currently doesnt work

    // Plan:
    // take the top point and middle point, and
    // the intersection point of the middle's
    // y-cor with the line between the top and
    // the bottom. Iterate downwards, drawing
    // horizontal lines.
    // Then, do the same with the middle point
    // and the bottom point, except that in
    // this case, you iterate until you hit the
    // bottom's y cor.

    int deltatm = ty - my;
    int deltamb = my - by;
    int deltatb = ty - by;

    int threstm = tx - mx;
    int thresmb = mx - bx;
    int threstb = tx - bx;

    int acctm = 0;
    int accbm = 0;
    int acctb = 0;

    pixel = SDL_MapRGB(surface->format, 20,255,200);

    int xi, xf, x, y; // the current pixel to be printed

    if (ty == my) { // if the top triangle is just a row
        x = tx;
        if (mx > tx) {
            xi = tx;
            xf = mx;
        } else {
            xi = mx;
            xf = tx;
        }
        y = ty;
        if (xf > xi) {
            increment = 1;
        } else {
            increment = -1;
        }
        // draw pixels across the row
        for (x = xi; x != xf; x+= increment) {
            // draw pixel at x,y of color r, g, b

            if (x1 > 0 && y1 > 0 && x1 < D_W && y1 < D_H)
                put_pixel(surface, x, y, pixel);
        }
    } else { // if the top triangle is not just a row
        x = tx;
        xi = x;
        xf = x;
        y = ty;
        if (xf > xi) {
            increment = -1;
        } else {
            increment = 1;
        }
        while (y < my) { // while the pixel is in the top half of the triangle
            // draw pixels acroos the row

            if (acctm > abs(threstm)) {
                xi += acctm / threstm;
                acctm = acctm % threstm; // might give an error, since threstm can be negative
            }
            if (acctb > abs(threstb)) {
                xf += acctb / threstb;
                acctb = acctb % threstb;     // might give an error, since threstb can be negative
            }
            printf("%d, %d", xi, xf);
            for (x = xi; x != xf; x+= increment) {
                // draw pixel at x,y of color r, g, b
                printf("%d, from %d to %d at %d:y\n", x, xi, xf, y);
                if (x1 > 0 && y1 > 0 && x1 < D_W && y1 < D_H)
                    put_pixel(surface, x, y, pixel);
            }

            y++;
            acctm += deltatm;
            acctb += deltatb;
        }
    }

    printf("top half of  triang done\n");

    if (my == by) { // if the bottom triangle is just a row
        x = bx;
        if (bx < mx) {
            xi = bx;
            xf = mx;
        }
        else {
            xi = mx;
            xf = bx;
        }
        y = my;
        if (xf > xi) {
            increment = 1;
        } else {
            increment = -1;
        }
        // draw pixels across the row
        for (x = xi; x != xf; x+= increment) {
            // draw pixel at x,y of color r, g, b
            printf("one line!\n");

            printf("%d, from %d to %d at %d:y\n", x, xi, xf,y);

            if (x1 > 0 && y1 > 0 && x1 < D_W && y1 < D_H)
                put_pixel(surface, x, y, pixel);

        }
    } else { // if the bottom triangle is not just a row
        x = bx;
        xi = x;
        xf = x;
        y = by;
        if (xf > xi) {
            increment = 1;
        } else {
            increment = -1;
        }
        while (y < by) {
            // draw pixels across the row

            if (accbm > abs(thresmb)) {
                xi += accbm / thresmb;
                accbm = accbm % thresmb; // might give an error, since threstm can be negative
            }
            if (acctb > abs(thresmb)) {
                xf += acctb / threstb;
                acctb = acctb % threstb;     // might give an error, since threstb can be negative
            }

            for (x = xi;x != xf;x += increment) {
                // draw pixel at x,y of color r, g, b
                printf("%d, from %d to %d at %d:y\n", x, xi, xf, y);

                if (x1 > 0 && y1 > 0 && x1 < D_W && y1 < D_H)
                    put_pixel(surface, x, y, pixel);

            }

            y++;
            acctm += deltatm;
            acctb += deltatb;
        }
    }
    printf("bottom half of triang done\n");



}



void draw() {

    int ii = 0;
    update_view();

    int* screenverticies[2];
    screenverticies[0] = malloc(sizeof(int) * mat4_columns(dmatrix));
    screenverticies[1] = malloc(sizeof(int) * mat4_columns(dmatrix));

    while (ii < mat4_columns(dmatrix)) {


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

        if (isvisible   (p1,p2,p3,rx,0-ry,rz,0)) {
            scanline_triangle(screen, x1,y1,x2,y2,x3,y3, mat4_get(cmatrix,0,ii), mat4_get(cmatrix,1,ii), mat4_get(cmatrix,2,ii));
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

