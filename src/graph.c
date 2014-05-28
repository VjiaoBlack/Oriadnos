#include "graph.h"


Uint32 get_pixel(SDL_Surface *surface, int x, int y) {

    // assumes bytes per pixel = 3
    int bpp = 3;

    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp + 1;

    if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
        return p[0] << 16 | p[1] << 8 | p[2];
    } else {                                  // 0 is red, 8 is green, 16 is blue
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

    if (ty == my) {
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
    } else {
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

    if (my == by) {
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
    } else {
        x = bx;
        xi = x;
        xf = x;
        y = by;
        if (xf > xi) {
            increment = 1;
        } else {
            increment = -1;
        }
        while (y > my) {
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

            y--;
            acctm += deltatm;
            acctb += deltatb;
        }
    }
    printf("bottom half of triang done\n");



}
