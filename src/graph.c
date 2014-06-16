#include "graph.h"

Uint32 get_pixel(image_t* image, int x, int y) {
    pixel_t *p = image->pixels + (y * image->width) + (image->width - x - 1);
    return SDL_MapRGB(screen->format, p->r, p->g, p->b);
}

void put_pixel(int x, int y, Uint32 pixel) {
    Uint8 *p = (Uint8*) screen->pixels + y * screen->pitch + x * 4;
    *(Uint32*)p = pixel;
}

void add_texture_rect(int texture, double p1[4], double p2[4], double p3[4], double p4[4]) {
    images = (int*) realloc(images, sizeof(int) * ++numimages);
    images[numimages - 1] = texture;

    mat4_add_column(ematrix, p1);
    mat4_add_column(ematrix, p2);
    mat4_add_column(ematrix, p3);
    mat4_add_column(ematrix, p4);
}

void add_wall_part(int x1, int y1, int z1, int x2, int y2, int z2) {
    add_texture_rect(WALL, (double[4]) {x1, y1, z1, 1},
                           (double[4]) {x2, y1, z2, 1},
                           (double[4]) {x2, y2, z2, 1},
                           (double[4]) {x1, y2, z1, 1});

    int x, z;
    for (x = (x1 > x2 ? x2 : x1); x <= (x1 > x2 ? x1 : x2); x++) {
        for (z = (z1 > z2 ? z2 : z1); z <= (z1 > z2 ? z1 : z2); z++)
            collision_map[WORLD_H / 2 - z][WORLD_W / 2 + x] = 1;
    }
}

void add_wall(int x1, int z1, int x2, int z2) {
    int i;
    if (z1 == z2) {
        if (x2 > x1) {
            for (i = x1; i < x2; i += 2)
                add_wall_part(i, 1, z1, i + 2, -1, z1);
        }
        else if (x2 < x1) {
            for (i = x1; i > x2; i -= 2)
                add_wall_part(i, 1, z1, i - 2, -1, z1);
        }
    }
    else if (x1 == x2) {
        if (z2 > z1) {
            for (i = z1; i < z2; i += 2)
                add_wall_part(x1, 1, i, x1, -1, i + 2);
        }
        else if (z2 < z1) {
            for (i = z1; i > z2; i -= 2)
                add_wall_part(x1, 1, i, x1, -1, i - 2);
        }
    }
    else {
        printf("Error: unspported wall coordinates: (%d, %d), (%d, %d)\n",
               x1, z1, x2, z2);
        exit(1);
    }
}

void add_floor_part(int x1, int y1, int z1, int x2, int y2, int z2) {
    add_texture_rect(FLOOR, (double[4]) {x1, y1, z1, 1},
                            (double[4]) {x2, y1, z1, 1},
                            (double[4]) {x2, y2, z2, 1},
                            (double[4]) {x1, y2, z2, 1});
}

void add_floor(int x1, int z1, int x2, int z2, int y) {
    int xi = (x1 > x2) ? x2 : x1;
    int xf = (x1 > x2) ? x1 : x2;
    int x = xi;
    int zi = (z1 > z2) ? z2 : z1;
    int zf = (z1 > z2) ? z1 : z2;
    int z = zi;

    while (x < xf) { // should add planes, but only adds strips. Whatever
        while (z < zf) {
            if (zf - z == 1) {
                if (xf - x == 1)
                    add_floor_part(x, y, z, x + 1, y, z + 1);
                else
                    add_floor_part(x, y, z, x + 2, y, z + 1);
                z++;
            } else {
                if (xf - x == 1)
                    add_floor_part(x, y, z, x + 1, y, z + 2);
                else
                    add_floor_part(x, y, z, x + 2, y, z + 2);
                z += 2;
            }
        }
        x += 2;
    }
}

void draw() {
    update_view();
    if (flicker < .7) {
        inc = 1;
    }
    if (flicker > .95) {
        inc = 0;
    }

    if (inc) {
        flicker += .03;
    } else {
        flicker -= .03;
    }

    int zi, zj;
    for (zi = 0; zi < D_H; zi++) {
        for (zj = 0; zj < D_W; zj++)
            zbuf[zi][zj] = DBL_MAX;
    }

    int current_image = 0;
    image_t* image;

    int ii;
    for (ii = 0; ii < mat4_columns(dmatrix); ii += 4) {
        switch (images[current_image++]) {
            case WALL:
                image = &wall;
                break;
            case FLOOR:
                image = &flor;
                break;
        }

        double x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;

        // the points in world coordinates (to calculate visibility)
        // p1 is top left, numbers increase clockwise.
        double p1[3], p2[3], p3[3], p4[3];

        z1 = mat4_get(dmatrix, 2, ii);
        z2 = mat4_get(dmatrix, 2, ii + 1);
        z3 = mat4_get(dmatrix, 2, ii + 2);
        z4 = mat4_get(dmatrix, 2, ii + 3);

        // test visibility: rect fully behind screen
        if (z1 >= Z_OFF && z2 >= Z_OFF && z3 >= Z_OFF && z4 >= Z_OFF)
            continue;

        p1[0] = mat4_get(dmatrix, 0, ii);
        p1[1] = mat4_get(dmatrix, 1, ii);
        p1[2] = z1;
        p2[0] = mat4_get(dmatrix, 0, ii + 1);
        p2[1] = mat4_get(dmatrix, 1, ii + 1);
        p2[2] = z2;
        p3[0] = mat4_get(dmatrix, 0, ii + 2);
        p3[1] = mat4_get(dmatrix, 1, ii + 2);
        p3[2] = z3;
        p4[0] = mat4_get(dmatrix, 0, ii + 3);
        p4[1] = mat4_get(dmatrix, 1, ii + 3);
        p4[2] = z4;
        // test visibility: rect facing backwards

        if (!images[current_image-1]){
            if (!isvisible(p1, p2, p3, X_OFF, -Y_OFF, Z_OFF, 0))
                continue;
        }

        #define scale_coords(p, x, y, z) \
            if (z == Z_OFF)  { \
                x =  p[0] + D_W / 2; \
                y = -p[1] + D_H / 2; \
            } \
            else { \
                x =  p[0] * Z_OFF / (Z_OFF - z) + D_W / 2; \
                y = -p[1] * Z_OFF / (Z_OFF - z) + D_H / 2; \
            }

        scale_coords(p1, x1, y1, z1);
        scale_coords(p2, x2, y2, z2);
        scale_coords(p3, x3, y3, z3);
        scale_coords(p4, x4, y4, z4);

        scanline_texture(image, x1, y1, Z_OFF - z1, x2, y2, Z_OFF - z2, x3, y3, Z_OFF - z3,
                         0, 0, image->width - 1, 0, image->width - 1, image->height - 1);
        scanline_texture(image, x3, y3, Z_OFF - z3, x4, y4, Z_OFF - z4, x1, y1, Z_OFF - z1,
                         image->width - 1, image->height - 1, 0, image->height - 1, 0, 0);
    }

}

int point_in_texture(image_t* texture, int x, int y) {
    return (x >= 0 && x < texture->width && y >= 0 && y < texture->height);
}

Uint32 shade_pixel(Uint32 pixel, double z) {
    #if ENABLE_SHADING
        Uint8 r, g, b;
        double dist = z > 750 ? 0 : 1 - z / 750;
        dist *= flicker;
        SDL_GetRGB(pixel, screen->format, &r, &g, &b);
        return SDL_MapRGB(screen->format, dist * r, dist * g, dist * b);
    #else
        return pixel;
    #endif
}

// Based on code by Mikael Kalms
// http://www.lysator.liu.se/~mikaelk/doc/perspectivetexture/

void scanline_texture(image_t *texture,
                      double x1, double y1, double z1,
                      double x2, double y2, double z2,
                      double x3, double y3, double z3,
                      double u1, double v1,
                      double u2, double v2,
                      double u3, double v3) {
    double iz1, uiz1, viz1, iz2, uiz2, viz2, iz3, uiz3, viz3;
    double dxdy1, dxdy2, dxdy3;
    double tempf;
    double denom;
    double dy;
    int y1i, y2i, y3i;
    int side;

    // Shift XY coordinate system (+0.5, +0.5) to match the subpixeling
    //  technique

    x1 = x1 + 0.5;
    y1 = y1 + 0.5;
    x2 = x2 + 0.5;
    y2 = y2 + 0.5;
    x3 = x3 + 0.5;
    y3 = y3 + 0.5;

    // Calculate alternative 1/Z, U/Z and V/Z values which will be
    //  interpolated

    iz1 = 1 / z1;
    iz2 = 1 / z2;
    iz3 = 1 / z3;
    uiz1 = u1 * iz1;
    viz1 = v1 * iz1;
    uiz2 = u2 * iz2;
    viz2 = v2 * iz2;
    uiz3 = u3 * iz3;
    viz3 = v3 * iz3;

    // Sort the vertices in ascending Y order

    #define swap(x, y) tempf = x; x = y; y = tempf;
    if (y1 > y2) {
        swap(x1, x2);
        swap(y1, y2);
        swap(iz1, iz2);
        swap(uiz1, uiz2);
        swap(viz1, viz2);
    }
    if (y1 > y3) {
        swap(x1, x3);
        swap(y1, y3);
        swap(iz1, iz3);
        swap(uiz1, uiz3);
        swap(viz1, viz3);
    }
    if (y2 > y3) {
        swap(x2, x3);
        swap(y2, y3);
        swap(iz2, iz3);
        swap(uiz2, uiz3);
        swap(viz2, viz3);
    }
    #undef swap

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

            scanline_texture_segment(texture, y1i, y2i);
        }
        if (y2i < y3i) {  // Draw lower segment if possibly visible
            // Set right edge X-slope and perform subpixel pre-
            //  stepping

            xb = x2 + (1 - (y2 - y2i)) * dxdy3;
            dxdyb = dxdy3;

            scanline_texture_segment(texture, y2i, y3i);
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

            scanline_texture_segment(texture, y1i, y2i);
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

            scanline_texture_segment(texture, y2i, y3i);
        }
    }
}

inline void scanline_texture_segment(image_t* texture, int y1, int y2) {
    int x1, x2, yextra = 0;
    double z, u, v, dx;
    double iz, uiz, viz;

    #define advance_y_vars(delta) \
        xa += dxdya * (delta);\
        xb += dxdyb * (delta);\
        iza += dizdya * (delta);\
        uiza += duizdya * (delta);\
        viza += dvizdya * (delta);

    // Fast math for out-of-bounds coordinates

    if ((y1 < 0 && y2 < 0) || (y1 >= D_H && y2 >= D_H)) {
        yextra = y2 - y1;
        y1 = y2 = 0;
    }
    if (y1 >= D_H) {
        advance_y_vars(y1 - D_H + 1);
        y1 = D_H - 1;
    }
    if (y2 < 0) {
        yextra = -y2;
        y2 = 0;
    }
    if (y1 < 0) {
        advance_y_vars(-y1);
        y1 = 0;
    }
    if (y2 >= D_H) {
        yextra = y2 - D_H + 1;
        y2 = D_H - 1;
    }

    while (y1 < y2) {  // Loop through all lines in the segment
        if (xb >= 0 && xa < D_W) {
            x1 = xa;
            x2 = xb;

            // Perform subtexel pre-stepping on 1/Z, U/Z and V/Z

            dx = 1 - (xa - x1);
            iz = iza + dx * dizdx;
            uiz = uiza + dx * duizdx;
            viz = viza + dx * dvizdx;

            // Fast math for out-of-bounds coordinates

            if (x1 < 0) {
                iz += dizdx * -x1;
                uiz += duizdx * -x1;;
                viz += dvizdx * -x1;;
                x1 = 0;
            }
            if (x2 >= D_W)
                x2 = D_W - 1;

            while (x1++ < x2) {  // Draw horizontal line
                // Calculate U and V from 1/Z, U/Z and V/Z

                z = 1 / iz;
                u = uiz * z;
                v = viz * z;

                // Copy pixel from texture to screen

                if (point_in_texture(texture, (int) u, (int) v)) {
                    if (z < zbuf[y1][x1] && z >= 0) {
                        Uint32 pixel = get_pixel(texture, (int) u, (int) v);
                        put_pixel(x1, y1, shade_pixel(pixel, z));
                        zbuf[y1][x1] = z;
                    }
                }

                // Step 1/Z, U/Z and V/Z horizontally

                iz += dizdx;
                uiz += duizdx;
                viz += dvizdx;
            }
        }

        // Step along both edges

        advance_y_vars(1);
        y1++;
    }

    if (yextra) {
        advance_y_vars(yextra);
    }
}
