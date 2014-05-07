#include "graph.h"


void put_pixel(SDL_Surface* surface, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    Uint32 pixel = SDL_MapRGB(surface->format, r,g,b);
    int bpp = surface->format->BytesPerPixel;

    Uint8 *p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
    switch (bpp) {
        case 1:
            *p = pixel;
            break;
        case 2:
            *(Uint16*) p = pixel;
            break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;
        case 4:
            *(Uint32*)p = pixel;
            break;
    }
}

void draw_line(SDL_Surface* surface, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b) {
    int acc = 0,
        delta = 0,
        x, y, up, right;
        // partnerx,
        // partnery;
        // aliased lines

        if (x1 == x2 && y1 == y2) {
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
