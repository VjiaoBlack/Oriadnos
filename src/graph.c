#include "graph.h"


Uint32 get_pixel(SDL_Surface *surface, int x, int y) {
    // TODO: 894 is placeholder for surface-width
    Uint8 *p = (Uint8*) surface->pixels + y * surface->pitch + (894 - x) * 4;
    return *(Uint32*)p;
}

void put_pixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {
    Uint8 *p = (Uint8*) surface->pixels + y * surface->pitch + x * 4;
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

void add_wall(int x1, int y1, int z1, int x2, int y2, int z2) {
    double* p1 = malloc(sizeof(double) * 4);
    double* p2 = malloc(sizeof(double) * 4);
    double* p3 = malloc(sizeof(double) * 4);
    double* p4 = malloc(sizeof(double) * 4);

    p1[0] = x1;
    p1[1] = y1;
    p1[2] = z1;
    p1[3] = 1;
    p2[0] = x2;
    p2[1] = y1;
    p2[2] = z2;
    p2[3] = 1;
    p3[0] = x2;
    p3[1] = y2;
    p3[2] = z2;
    p3[3] = 1;
    p4[0] = x1;
    p4[1] = y2;
    p4[2] = z1;
    p4[3] = 1;

    printf("added wall\n");
    mat4_add_column(texturematrix, p1);
    mat4_add_column(texturematrix, p2);
    mat4_add_column(texturematrix, p3);
    mat4_add_column(texturematrix, p4);

    free(p1);
    free(p2);
    free(p3);
    free(p4);

    return;
}

void organize(int x1, int y1, int x2, int y2, int x3, int y3, double z1, double z2, double z3, int* tx, int* ty, int* mx, int* my, int* bx, int* by, double* tz, double* mz, double* bz) {
    if (y1 < y2) {
        if (y2 < y3) {
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
            if (y1 < y3) {
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
        if (y1 < y3) {
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
            if (y2 < y3) {
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

        if (y < 0 || y >= D_H)
            continue;
        // if (y >= D_H)
        //     break;

        zi = az + ((((double) (y - ay)) / (by - ay)) * (blz - az));
        zf = az + ((((double) (y - ay)) / (by - ay)) * (brz - az));

        for (x = (xi > 0 ? xi : 0); x <= (xf <= D_W ? xf : D_W - 1); x++) {
            if (y > 0 && y < D_H && x > 0 && x < D_W) {
                if (xi == xf)
                    z = zi;
                else
                    z = zi + ((zf - zi) * ((double) (x - xi)) / (xf - xi));
                if (1/z < zbuf[y][x]) {
                    c = (z > 0.002 ? 1 : z / 0.002);
                    pixel = SDL_MapRGB(surface->format, (int) (r * c), (int) (g * c), (int) (b * c));
                    put_pixel(surface, x, y, pixel);
                    zbuf[y][x] = 1/z;
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
            if (1/z < zbuf[y][x]) {
                c = (z > 0.002 ? 1 : z / 0.002);
                pixel = SDL_MapRGB(surface->format, (int) (r * c), (int) (g * c), (int) (b * c));
                put_pixel(surface, x, y, pixel);
                zbuf[y][x] = 1/z;
            }
        }
    }
}

void scanline_triangle(SDL_Surface* surface, int x1, int y1, int x2, int y2, int x3, int y3, double z1, double z2, double z3, Uint8 r, Uint8 g, Uint8 b) { // to screen coordinates.
    int tx, ty, mx, my, bx, by, px; // top, middle, bottom
    double tz, mz, bz, pz;

    organize(x1,y1,x2,y2,x3,y3,z1,z2,z3,&tx,&ty,&mx,&my,&bx,&by,&tz,&mz,&bz);

    // intersection of mid-y with t-b
    if (ty == by) {
        px = tx;
        pz = tz;
    }
    else {
        px = tx - (tx - bx) * (ty - my) / (ty - by);
        pz = tz - (tz - bz) * (ty - my) / (ty - by);
    }

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

        // initializes screen verticies
        screenverticies[0][ii] = -1;
        screenverticies[1][ii] = -1;
        screenverticies[0][ii+1] = -1;
        screenverticies[1][ii+1] = -1;
        screenverticies[0][ii+2] = -1;
        screenverticies[1][ii+2] = -1;

        int rx, ry, rz, x1, y1, z1, x2, y2, z2, x3, y3, z3;
        int a = 0, b = 0, c = 0;

        // the points in world coordinates (to calculate visibility)
        double p1[3], p2[3], p3[3];

        // eye coordinates in pixel coordinates
        rx = (int) (((0 - EYE_X) * D_W) / (S_W)  + D_W / 2);
        ry = (int) (((0 - EYE_Y) * D_H) / (S_H)  + D_H / 2);
        rz = (int) ((EYE_Z * D_W) / (S_W));

        // world coordinates of points relative to eye
        x1 = rx - (int) mat4_get(dmatrix, 0, ii);
        y1 = ry + (int) mat4_get(dmatrix, 1, ii);
        z1 =            mat4_get(dmatrix, 2, ii);
        x2 = rx - (int) mat4_get(dmatrix, 0, ii+1);
        y2 = ry + (int) mat4_get(dmatrix, 1, ii+1);
        z2 =            mat4_get(dmatrix, 2, ii+1);
        x3 = rx - (int) mat4_get(dmatrix, 0, ii+2);
        y3 = ry + (int) mat4_get(dmatrix, 1, ii+2);
        z3 =            mat4_get(dmatrix, 2, ii+2);

        // makes sure that the object is supposed to be displayed.
        if (z1 < 999) {
            a = 1;
        } else {
            a = -1;
        }
        if (z2 < 999) {
            b = 1;
        } else {
            b = -1;
        }
        if (z3 < 999) {
            c = 1;
        } else {
            c = -1;
        }


        // these are the points in 3d space
        if (a == 1 && b == 1 && c == 1) {
            if ((rz - z1) > 1) {
                p1[0] = mat4_get(dmatrix, 0, ii);
                p1[1] = mat4_get(dmatrix, 1, ii);
                p1[2] = mat4_get(dmatrix, 2, ii);

                // scaling function
                x1 = rx - x1 * rz / (rz - z1) + D_W / 2;
                y1 = ry - y1 * rz / (rz - z1) + D_H / 2;
                screenverticies[0][ii] = x1;
                screenverticies[1][ii] = y1;
            }
            if ((rz - z2) > 1) {
                p2[0] = mat4_get(dmatrix, 0, ii+1);
                p2[1] = mat4_get(dmatrix, 1, ii+1);
                p2[2] = mat4_get(dmatrix, 2, ii+1);

                // scaling function
                x2 = rx - x2 * rz / (rz - z2) + D_W / 2;
                y2 = ry - y2 * rz / (rz - z2) + D_H / 2;
                screenverticies[0][ii+1] = x2;
                screenverticies[1][ii+1] = y2;
            }
            if ((rz - z3) > 1) {
                p3[0] = mat4_get(dmatrix, 0, ii+2);
                p3[1] = mat4_get(dmatrix, 1, ii+2);
                p3[2] = mat4_get(dmatrix, 2, ii+2);

                // scaling function
                x3 = rx - x3 * rz / (rz - z3) + D_W / 2;
                y3 = ry - y3 * rz / (rz - z3) + D_H / 2;
                screenverticies[0][ii+2] = x3;
                screenverticies[1][ii+2] = y3;
            }

            // _TEST_
            /* ***** BEGIN TEST CODE FOR TRIANGLE COORDS - REMOVE WHEN NECESSARY ***** */
            // double t1 = mat4_get(cmatrix,0,ii), t2 = mat4_get(cmatrix,1,ii), t3 = mat4_get(cmatrix,2,ii);
            // char* tmp;
            // if (t1 == 255 && t2 == 255 && t3 == 0)
            //     tmp = "yellow ";
            // else if (t1 == 255 && t2 == 0 && t3 == 255)
            //     tmp = "magenta";
            // else if (t1 == 0 && t2 == 255 && t3 == 255)
            //     tmp = "cyan   ";
            // else
            //     tmp = "unknown";
            // printf("%s -> %0.f, %0.f, %0.f\n", tmp, 1000-p1[2],1000-p2[2],1000-p3[2]);
            /* ***** END TEST CODE FOR TRIANGLE COORDS - REMOVE WHEN NECESSARY ***** */

            #define RZD (((double) (EYE_Z * D_W)) / (S_W))
            scanline_triangle(screen, x1,y1,x2,y2,x3,y3,1/(RZD-p1[2]),1/(RZD-p2[2]),1/(RZD-p3[2]), mat4_get(cmatrix,0,ii), mat4_get(cmatrix,1,ii), mat4_get(cmatrix,2,ii));
        }

        ii += 3;
    }

    ii = 0; // resets counter variable
    while (ii < mat4_columns(texturdmatrix)) {
        // to be honest, I'm not sure what screenverticies is for, so I just deleted it in this one.

        int rx, ry, rz, x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4; // 4 coordinates needed.
        // p1 is top left, numbers increase clockwise.

        int a = 0, b = 0, c = 0, d = 0;

        // the points in world coordinates (to calculate visibility)
        double p1[3], p2[3], p3[3], p4[3];

        // eye coordinates in pixel coordinates
        rx = (int) (((0 - EYE_X) * D_W) / (S_W)  + D_W / 2);
        ry = (int) (((0 - EYE_Y) * D_H) / (S_H)  + D_H / 2);
        rz = (int) ((EYE_Z * D_W) / (S_W));

        // _TEST_
        // mat4_print(texturdmatrix);
        // world coordinates of points relative to eye
        x1 = rx - (int) mat4_get(texturdmatrix, 0, ii);
        y1 = ry + (int) mat4_get(texturdmatrix, 1, ii);
        z1 =            mat4_get(texturdmatrix, 2, ii);
        x2 = rx - (int) mat4_get(texturdmatrix, 0, ii+1);
        y2 = ry + (int) mat4_get(texturdmatrix, 1, ii+1);
        z2 =            mat4_get(texturdmatrix, 2, ii+1);
        x3 = rx - (int) mat4_get(texturdmatrix, 0, ii+2);
        y3 = ry + (int) mat4_get(texturdmatrix, 1, ii+2);
        z3 =            mat4_get(texturdmatrix, 2, ii+2);
        x4 = rx - (int) mat4_get(texturdmatrix, 0, ii+3);
        y4 = ry + (int) mat4_get(texturdmatrix, 1, ii+3);
        z4 =            mat4_get(texturdmatrix, 2, ii+3);

        // makes sure that the object is supposed to be displayed.
        if (z1 < 1000) {
            a = 1;
        } else {
            a = -1;
        }
        if (z2 < 1000) {
            b = 1;
        } else {
            b = -1;
        }
        if (z3 < 1000) {
            c = 1;
        } else {
            c = -1;
        }
        if (z4 < 1000) {
            d = 1;
        } else {
            d = -1;
        }

        // these are the points in 3d space
        if (a == 1 && b == 1 && c == 1 && d == 1) {
            if ((rz - z1) > 1) {
                p1[0] = mat4_get(texturdmatrix, 0, ii);
                p1[1] = mat4_get(texturdmatrix, 1, ii);
                p1[2] = mat4_get(texturdmatrix, 2, ii);

                // scaling function
                x1 = rx - x1 * rz / (rz - z1) + D_W / 2;
                y1 = ry - y1 * rz / (rz - z1) + D_H / 2;
            }
            if ((rz - z2) > 1) {
                p2[0] = mat4_get(texturdmatrix, 0, ii+1);
                p2[1] = mat4_get(texturdmatrix, 1, ii+1);
                p2[2] = mat4_get(texturdmatrix, 2, ii+1);

                // scaling function
                x2 = rx - x2 * rz / (rz - z2) + D_W / 2;
                y2 = ry - y2 * rz / (rz - z2) + D_H / 2;
            }
            if ((rz - z3) > 1) {
                p3[0] = mat4_get(texturdmatrix, 0, ii+2);
                p3[1] = mat4_get(texturdmatrix, 1, ii+2);
                p3[2] = mat4_get(texturdmatrix, 2, ii+2);

                // scaling function
                x3 = rx - x3 * rz / (rz - z3) + D_W / 2;
                y3 = ry - y3 * rz / (rz - z3) + D_H / 2;
            }
            if ((rz - z4) > 1) {
                p4[0] = mat4_get(texturdmatrix, 0, ii+3);
                p4[1] = mat4_get(texturdmatrix, 1, ii+3);
                p4[2] = mat4_get(texturdmatrix, 2, ii+3);

                // scaling function
                x4 = rx - x4 * rz / (rz - z4) + D_W / 2;
                y4 = ry - y4 * rz / (rz - z4) + D_H / 2;
            }

            if (isvisible(p1,p2,p3,rx,0-ry,rz,0)) {
                struct TPolytri poly1 = {x1, y1, RZD - z1, x2, y2, RZD - z2, x3, y3, RZD - z3, 0, 0, 894, 0, 894, 894, wall},
                                poly2 = {x3, y3, RZD - z3, x4, y4, RZD - z4, x1, y1, RZD - z1, 894, 894, 0, 894, 0, 0, wall};

                drawtpolyperspsubtri(&poly1);
                drawtpolyperspsubtri(&poly2);
            }
        }

        ii += 4;
    }

}

int point_in_screen(int x, int y) {
    return (x > 0 && x < D_W && y > 0 && y < D_H);
}

// Based on code by Mikael Kalms
// http://www.lysator.liu.se/~mikaelk/doc/perspectivetexture/

void drawtpolyperspsubtriseg(int y1, int y2) {
    int x1, x2;
    float z, u, v, dx;
    float iz, uiz, viz;

    while (y1 < y2) {  // Loop through all lines in the segment
        x1 = xa;
        x2 = xb;

        // Perform subtexel pre-stepping on 1/Z, U/Z and V/Z

        dx = 1 - (xa - x1);
        iz = iza + dx * dizdx;
        uiz = uiza + dx * duizdx;
        viz = viza + dx * dvizdx;

        while (x1++ < x2) {  // Draw horizontal line
            // Calculate U and V from 1/Z, U/Z and V/Z

            z = 1 / iz;
            u = uiz * z;
            v = viz * z;

            // Copy pixel from texture to screen

            if (x1 >= 0 && x1 < D_W && y1 >= 0 && y1 < D_H) {
                if (z < zbuf[y1][x1]) {
                    Uint32 pixel = get_pixel(texture, (u < 894 ? (int)u : 499), (v < 894 ? (int)v : 499));
                    put_pixel(screen, x1, y1, pixel);
                    zbuf[y1][x1] = z;
                }
            }

            // Step 1/Z, U/Z and V/Z horizontally

            iz += dizdx;
            uiz += duizdx;
            viz += dvizdx;
        }

        // Step along both edges

        xa += dxdya;
        xb += dxdyb;
        iza += dizdya;
        uiza += duizdya;
        viza += dvizdya;

        y1++;
    }
}

void drawtpolyperspsubtri(struct TPolytri *poly) {
    float x1, y1, x2, y2, x3, y3;
    float iz1, uiz1, viz1, iz2, uiz2, viz2, iz3, uiz3, viz3;
    float dxdy1, dxdy2, dxdy3;
    float tempf;
    float denom;
    float dy;
    int y1i, y2i, y3i;
    int side;

    // Shift XY coordinate system (+0.5, +0.5) to match the subpixeling
    //  technique

    x1 = poly->x1 + 0.5;
    y1 = poly->y1 + 0.5;
    x2 = poly->x2 + 0.5;
    y2 = poly->y2 + 0.5;
    x3 = poly->x3 + 0.5;
    y3 = poly->y3 + 0.5;

    // Calculate alternative 1/Z, U/Z and V/Z values which will be
    //  interpolated

    iz1 = 1 / poly->z1;
    iz2 = 1 / poly->z2;
    iz3 = 1 / poly->z3;
    uiz1 = poly->u1 * iz1;
    viz1 = poly->v1 * iz1;
    uiz2 = poly->u2 * iz2;
    viz2 = poly->v2 * iz2;
    uiz3 = poly->u3 * iz3;
    viz3 = poly->v3 * iz3;

    texture = poly->texture;

    // Sort the vertices in ascending Y order

    #define swapfloat(x, y) tempf = x; x = y; y = tempf;
    if (y1 > y2) {
        swapfloat(x1, x2);
        swapfloat(y1, y2);
        swapfloat(iz1, iz2);
        swapfloat(uiz1, uiz2);
        swapfloat(viz1, viz2);
    }
    if (y1 > y3) {
        swapfloat(x1, x3);
        swapfloat(y1, y3);
        swapfloat(iz1, iz3);
        swapfloat(uiz1, uiz3);
        swapfloat(viz1, viz3);
    }
    if (y2 > y3) {
        swapfloat(x2, x3);
        swapfloat(y2, y3);
        swapfloat(iz2, iz3);
        swapfloat(uiz2, uiz3);
        swapfloat(viz2, viz3);
    }
    #undef swapfloat

    y1i = y1;
    y2i = y2;
    y3i = y3;

    // Skip poly if it's too thin to cover any pixels at all

    if ((y1i == y2i && y1i == y3i) || ((int) x1 == (int) x2 && (int) x1 == (int) x3))
        return;

    // Calculate horizontal and vertical increments for UV axes (these
    //  calcs are certainly not optimal, although they're stable
    //  (handles any dy being 0)

    denom = ((x3 - x1) * (y2 - y1) - (x2 - x1) * (y3 - y1));

    if (!denom)     // Skip poly if it's an infinitely thin line
        return;

    denom = 1 / denom;  // Reciprocal for speeding up
    dizdx = ((iz3 - iz1) * (y2 - y1) - (iz2 - iz1) * (y3 - y1)) * denom;
    duizdx = ((uiz3 - uiz1) * (y2 - y1) - (uiz2 - uiz1) * (y3 - y1)) * denom;
    dvizdx = ((viz3 - viz1) * (y2 - y1) - (viz2 - viz1) * (y3 - y1)) * denom;
    dizdy = ((iz2 - iz1) * (x3 - x1) - (iz3 - iz1) * (x2 - x1)) * denom;
    duizdy = ((uiz2 - uiz1) * (x3 - x1) - (uiz3 - uiz1) * (x2 - x1)) * denom;
    dvizdy = ((viz2 - viz1) * (x3 - x1) - (viz3 - viz1) * (x2 - x1)) * denom;

    // Calculate X-slopes along the edges

    dxdy1 = (x2 - x1) / (y2 - y1);
    dxdy2 = (x3 - x1) / (y3 - y1);
    dxdy3 = (x3 - x2) / (y3 - y2);

    // Determine which side of the poly the longer edge is on

    side = dxdy2 > dxdy1;

    if (y1 == y2)
        side = x1 > x2;
    if (y2 == y3)
        side = x3 > x2;

    if (!side)  {  // Longer edge is on the left side
        // Calculate slopes along left edge

        dxdya = dxdy2;
        dizdya = dxdy2 * dizdx + dizdy;
        duizdya = dxdy2 * duizdx + duizdy;
        dvizdya = dxdy2 * dvizdx + dvizdy;

        // Perform subpixel pre-stepping along left edge

        dy = 1 - (y1 - y1i);
        xa = x1 + dy * dxdya;
        iza = iz1 + dy * dizdya;
        uiza = uiz1 + dy * duizdya;
        viza = viz1 + dy * dvizdya;

        if (y1i < y2i) {  // Draw upper segment if possibly visible
            // Set right edge X-slope and perform subpixel pre-
            //  stepping

            xb = x1 + dy * dxdy1;
            dxdyb = dxdy1;

            drawtpolyperspsubtriseg(y1i, y2i);
        }
        if (y2i < y3i) {  // Draw lower segment if possibly visible
            // Set right edge X-slope and perform subpixel pre-
            //  stepping

            xb = x2 + (1 - (y2 - y2i)) * dxdy3;
            dxdyb = dxdy3;

            drawtpolyperspsubtriseg(y2i, y3i);
        }
    }
    else {   // Longer edge is on the right side
        // Set right edge X-slope and perform subpixel pre-stepping

        dxdyb = dxdy2;
        dy = 1 - (y1 - y1i);
        xb = x1 + dy * dxdyb;

        if (y1i < y2i) {  // Draw upper segment if possibly visible
            // Set slopes along left edge and perform subpixel
            //  pre-stepping

            dxdya = dxdy1;
            dizdya = dxdy1 * dizdx + dizdy;
            duizdya = dxdy1 * duizdx + duizdy;
            dvizdya = dxdy1 * dvizdx + dvizdy;
            xa = x1 + dy * dxdya;
            iza = iz1 + dy * dizdya;
            uiza = uiz1 + dy * duizdya;
            viza = viz1 + dy * dvizdya;

            drawtpolyperspsubtriseg(y1i, y2i);
        }
        if (y2i < y3i) {  // Draw lower segment if possibly visible
            // Set slopes along left edge and perform subpixel
            //  pre-stepping

            dxdya = dxdy3;
            dizdya = dxdy3 * dizdx + dizdy;
            duizdya = dxdy3 * duizdx + duizdy;
            dvizdya = dxdy3 * dvizdx + dvizdy;
            dy = 1 - (y2 - y2i);
            xa = x2 + dy * dxdya;
            iza = iz2 + dy * dizdya;
            uiza = uiz2 + dy * duizdya;
            viza = viz2 + dy * dvizdya;

            drawtpolyperspsubtriseg(y2i, y3i);
        }
    }
}
