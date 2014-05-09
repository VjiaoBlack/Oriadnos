#include "graph.h"


Uint32 getpixel(SDL_Surface *surface, int x, int y) {
    // int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    int bpp = 4; // usually right
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    // switch(bpp) {
    // case 1:
    //     return *p;
    //     break;

    // case 2:
    //     return *(Uint16 *)p;
    //     break;

    // case 3: // probably this.
    //     if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
    //         return p[0] << 16 | p[1] << 8 | p[2];
    //     else
            // return p[0] | p[1] << 8 | p[2] << 16;
    //     break;

    // case 4:
        return *(Uint32 *)p;
    //     break;

    // default:
    //     return 0;       /* shouldn't happen, but avoids warnings */
    // }
}

void put_pixel(SDL_Surface* surface, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    Uint32 pixel = SDL_MapRGB(surface->format, r,g,b);
    // int bpp = surface->format->BytesPerPixel;
    int bpp = 4; // usually right

    Uint8 *p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
    // switch (bpp) {
    //     case 1:
    //         *p = pixel;
    //         break;
    //     case 2:
    //         *(Uint16*) p = pixel;
    //         break;
    //     case 3: // probably this.
    //         if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
    //             p[0] = (pixel >> 16) & 0xff;
    //             p[1] = (pixel >> 8) & 0xff;
    //             p[2] = pixel & 0xff;
    //         } else {
                // p[0] = pixel & 0xff;
                // p[1] = (pixel >> 8) & 0xff;
                // p[2] = (pixel >> 16) & 0xff;
    //         }
    //         break;
    //     case 4:
            *(Uint32*)p = pixel;
    //         break;
    // }
}

void draw_line(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b) {
    int acc = 0,
        delta = 0,
        x, y, up, right;
        // partnerx,
        // partnery;
        // aliased lines

        if (x1 == x2 && y1 == y2) {
            if (x1 > 0 && y1 > 0 && x1 < D_W && y1 < D_H)
                put_pixel(surface, x1,y1,r,g,b);
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
                if (x > 0 && y > 0 && x < D_W && y < D_H)
                    put_pixel(surface, x,y,r,g,b);
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
                if (x > 0 && y > 0 && x < D_W && y < D_H)
                    put_pixel(surface,x,y,r,g,b);

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
