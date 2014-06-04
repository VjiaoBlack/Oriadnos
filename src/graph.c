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
    Uint8 *p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
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

void organize(int x1, int y1, int x2, int y2, int x3, int y3, double z1, double z2, double z3, int* tx, int* ty, int* mx, int* my, int* bx, int* by, double* tz, double* mz, double* bz) {
    if (y1 > y2) {
        if (y2 > y3) {
            *tx = x1;
            *ty = y1;
            *tz = z1;
            *mx = x2;
            *my = y2;
            *mz = z2;
            *bx = x3;
            *by = y3;
            *bz = z3;
        } else {
            if (y1 > y3) {
                *tx = x1;
                *ty = y1;
                *tz = z1;
                *mx = x3;
                *my = y3;
                *mz = z3;
                *bx = x2;
                *by = y2;
                *bz = z2;
            } else {
                *tx = x3;
                *ty = y3;
                *tz = z3;
                *mx = x1;
                *my = y1;
                *mz = z1;
                *bx = x2;
                *by = y2;
                *bz = z2;
            }
        }
    } else {
        if (y1 > y3) {
            *tx = x2;
            *ty = y2;
            *tz = z2;
            *mx = x1;
            *my = y1;
            *mz = z1;
            *bx = x3;
            *by = y3;
            *bz = z3;
        } else {
            if (y2 > y3) {
                *tx = x2;
                *ty = y2;
                *tz = z2;
                *mx = x3;
                *my = y3;
                *mz = z3;
                *bx = x1;
                *by = y1;
                *bz = z1;
            } else {
                *tx = x3;
                *ty = y3;
                *tz = z3;
                *mx = x2;
                *my = y2;
                *mz = z2;
                *bx = x1;
                *by = y1;
                *bz = z1;
            }
        }
    }
}

void half_scanline_triangle(SDL_Surface* surface, int ax, int ay, int by, int blx, int brx, double az, double blz, double brz, Uint8 r, Uint8 g, Uint8 b) { // to screen coordinates.

    Uint32 pixel;

    int deltal = abs(ax - blx);
    int deltar = abs(ax - brx);

    int thres = abs(ay - by);

    pixel = SDL_MapRGB(surface->format, r, g, b);

    int xi, xf, x, y, accl = 0, accr = 0;
    int diry = (ay < by) ? 1 : -1; // 1 if top triangle, -1 if bottom triangle
    double z, zi, zf;

    int dirl = (blx > ax) ? 1 : -1;
    int dirr = (brx > ax) ? 1 : -1;
    xi = ax;
    xf = ax;
    double c;  // test

    if (thres == 0) {
        return;
    }

    for (y = ay; y != by; y += diry) {
        accl += deltal;
        accr += deltar;

        while (accl >= thres) {
            accl -= thres;
            xi += dirl;
        }
        while (accr >= thres) {
            accr -= thres;
            xf += dirr;
        }

        zi = az + ((((double) (y - ay)) / (by - ay)) * (blz - az));
        zf = az + ((((double) (y - ay)) / (by - ay)) * (brz - az));

        for (x = xi; x <= xf; x++) {
            if (y > 0 && y < D_H && x > 0 && x < D_W) {
                if (xi == xf)
                    z = zi;
                else
                    z = zi + ((zf - zi) * ((double) (x - xi)) / (xf - xi));
                if (z < zbuf[y][x]) {
                    c = 1 - (z > 1500000 ? 1 : z / 1500000);
                    pixel = SDL_MapRGB(surface->format, (int) (r * c), (int) (g * c), (int) (b * c));
                    put_pixel(surface, x, y, pixel);
                    zbuf[y][x] = z;
                }
            }
        }
    }

    y = by;
    for (x = blx; x <= brx; x++) {
        if (y > 0 && y < D_H && x > 0 && x < D_W) {
            if (xi == xf)
                z = zi;
            else
                z = zi + ((zf - zi) * ((double) (x - xi)) / (xf - xi));
            if (z < zbuf[y][x]) {
                c = 1 - (z > 1500000 ? 1 : z / 1500000);
                pixel = SDL_MapRGB(surface->format, (int) (r * c), (int) (g * c), (int) (b * c));
                put_pixel(surface, x, y, pixel);
                zbuf[y][x] = z;
            }
        }
    }
}

void scanline_triangle(SDL_Surface* surface, int x1, int y1, int x2, int y2, int x3, int y3, double z1, double z2, double z3, Uint8 r, Uint8 g, Uint8 b) { // to screen coordinates.
    int tx, ty, mx, my, bx, by; // top, middle, bottom
    double tz, mz, bz;

    organize(x1,y1,x2,y2,x3,y3,z1,z2,z3,&tx,&ty,&mx,&my,&bx,&by,&tz,&mz,&bz);

    // intersection of mid-y with t-b
    int px = tx - (tx - bx) * (ty - my) / (ty - by);
    int pz = tz - (tz - bz) * (ty - my) / (ty - by);

    if (px > mx) {
        half_scanline_triangle(surface, tx, ty, my, mx, px, tz, mz, pz, r, g, b);
        half_scanline_triangle(surface, bx, by, my, mx, px, bz, mz, pz, r, g, b);
    } else {
        half_scanline_triangle(surface, tx, ty, my, px, mx, tz, pz, mz, r, g, b);
        half_scanline_triangle(surface, bx, by, my, px, mx, bz, pz, mz, r, g, b);
    }



}

void draw() {

    int ii = 0;
    update_view();

    int* screenverticies[2];
    screenverticies[0] = malloc(sizeof(int) * mat4_columns(dmatrix));
    screenverticies[1] = malloc(sizeof(int) * mat4_columns(dmatrix));

    int zi, zj;
    for (zi = 0; zi < D_H; zi++) {
        for (zj = 0; zj < D_W; zj++)
            zbuf[zi][zj] = DBL_MAX;
    }

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

            // COMPUTE DISTANCES TO POINTS FROM EYES
            double d1, d2, d3;
            #define INNER_DIST_3D(a1, b1, c1, a2, b2, c2) ((a1 - a2) * (a1 - a2) + (b1 - b2) * (b1 - b2) + (c1 - c2) * (c1 - c2))
            #define RXD (((double) (0 - EYE_X) * D_W) / (S_W)  + D_W / 2)
            #define RYD (((double) (0 - EYE_Y) * D_H) / (S_H)  + D_H / 2)
            #define RZD (((double) (EYE_Z * D_W)) / (S_W))

            d1 = INNER_DIST_3D(p1[0], p1[1], p1[2], RXD, RYD, RZD);
            d2 = INNER_DIST_3D(p2[0], p2[1], p2[2], RXD, RYD, RZD);
            d3 = INNER_DIST_3D(p3[0], p3[1], p3[2], RXD, RYD, RZD);

            scanline_triangle(screen, x1,y1,x2,y2,x3,y3,d1,d2,d3, mat4_get(cmatrix,0,ii), mat4_get(cmatrix,1,ii), mat4_get(cmatrix,2,ii));
            // if (a && b)
            //     draw_line(screen, x1, y1, x2, y2, 255, 255, 255);
            // if (b && c)
            //     draw_line(screen, x3, y3, x2, y2, 255, 255, 255);
            // if (a && c)
            //     draw_line(screen, x3, y3, x1, y1, 255, 255, 255);
        }

        ii += 3;
    }

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

