#include "trans.h"

void combine(Mat4* new) {
    tmatrix = mat4_mult(new, tmatrix);
}

void init() {
    tmatrix = identity();
    dmatrix = mat4_create(0);
    ematrix = mat4_create(0);
    stacksize = 0;
}

void push(Mat4* mat) {
    tstack = (Mat4**) realloc(tstack, sizeof(Mat4*)*++stacksize);
    tstack[stacksize-1] = mat4_copy(mat);
}

Mat4* pop() {
    if (stacksize < 1)
        return NULL;
    else {
        Mat4* ans = mat4_create(4);
        ans = mat4_copy(tstack[stacksize-1]);
        tstack[--stacksize] = NULL;
        tstack = (Mat4**) realloc(tstack,sizeof(Mat4*)*stacksize);
        return ans;
    }

}

int searchtrans(char* name) {
    // finds name inside of storagenames.
    int i;
    for (i = 0; i < namedsize; i++) {
        if (strcmp(name, storagenames[i]) == 0) {
            return i;
            break;
        }
    }
    return -1;
}

void save(Mat4* mat, char* name) {
    if (searchtrans(name) > -1) {
        return;
    }
    storagenames = (char**) realloc(storagenames, ++namedsize);
    storagenames[namedsize - 1] = malloc(sizeof(char*) * 1);
    strcpy(storagenames[namedsize - 1],name);

    namedtdict = (Mat4**) realloc(namedtdict, namedsize);
    namedtdict[namedsize - 1] = (Mat4*) malloc(sizeof(Mat4*) * 1);
    namedtdict[namedsize - 1] = mat4_copy(mat);


}

void restore(char* name) {
    int index = searchtrans(name);

    if (index > -1) {
        Mat4* mat = mat4_copy(namedtdict[index]);
        // printf("ultrua\n");
        tmatrix = mat4_copy(mat);
    }
}

// void addcurrentobject() {
//     // iterates through the omatrix, and adds its columns to ematrix, then resets the omatrix.
//     int c, r;
//     for (c=0; c<omatrix->cols; c++) {
//         double col[4];
//         for (r = 0; r < 4; r++) {
//             // printf("getting item %d, %d, until %d\n", r, c, omatrix->cols);
//             col[r] = mat4_get(omatrix,r,c);
//         }

//         mat4_add_column(ematrix,col);
//         // printf("added col.\n");

//     }


//     omatrix = mat4_create(0);
//     // printf("test\n");
//     return;
// }

void addtriangle(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, int r, int g, int b) {
    // adds these three points to the matrix.


    double* p1 = malloc(sizeof(double) * 4);
    double* p2 = malloc(sizeof(double) * 4);
    double* p3 = malloc(sizeof(double) * 4);

    p1[0] = x1;
    p1[1] = y1;
    p1[2] = z1;
    p1[3] = 1;
    p2[0] = x2;
    p2[1] = y2;
    p2[2] = z2;
    p2[3] = 1;
    p3[0] = x3;
    p3[1] = y3;
    p3[2] = z3;
    p3[3] = 1;

    mat4_add_column(ematrix, p1);
    mat4_add_column(ematrix, p2);
    mat4_add_column(ematrix, p3);

    double* p1c = malloc(sizeof(double) * 4);
    double* p2c = malloc(sizeof(double) * 4);
    double* p3c = malloc(sizeof(double) * 4);

    p1c[0] = r;
    p1c[1] = g;
    p1c[2] = b;
    p1c[3] = 255;
    p2c[0] = r;
    p2c[1] = g;
    p2c[2] = b;
    p2c[3] = 255;
    p3c[0] = r;
    p3c[1] = g;
    p3c[2] = b;
    p3c[3] = 255;

    free(p1);
    free(p2);
    free(p3);

    free(p1c);
    free(p2c);
    free(p3c);


    return;
}

Mat4* identity() {
    Mat4* matrix= mat4_create(4);
    mat4_set(matrix, 0, 0, 1);
    mat4_set(matrix, 1, 1, 1);
    mat4_set(matrix, 2, 2, 1);
    mat4_set(matrix, 3, 3, 1);
    return matrix;
}

void move(double x, double y, double z) {
    Mat4* new = identity();

    mat4_set(new, 0, 3, x);
    mat4_set(new, 1, 3, y);
    mat4_set(new, 2, 3, z);

    combine(new);
    return;
}

void scale(double x, double y, double z) {
    Mat4* new = identity();

    mat4_set(new, 0, 0, x);
    mat4_set(new, 1, 1, y);
    mat4_set(new, 2, 2, z);

    combine(new);

    return;
}

void rotate(char axis, int degrees) {
    double radians = degrees * M_PI / 180;
    double c = cos(radians);
    double s = sin(radians);
    Mat4* new = identity();

    switch(axis) {
        case 'x':
            mat4_set(new, 1, 1, c);
            mat4_set(new, 1, 2, 0.0 - s);
            mat4_set(new, 2, 1, s);
            mat4_set(new, 2, 2, c);
            break;
        case 'y':
            mat4_set(new, 0, 0, c);
            mat4_set(new, 2, 0, 0.0 - s);
            mat4_set(new, 0, 2, s);
            mat4_set(new, 2, 2, c);
            break;
        case 'z':
            mat4_set(new, 0, 0, c);
            mat4_set(new, 0, 1, 0.0 - s);
            mat4_set(new, 1, 0, s);
            mat4_set(new, 1, 1, c);
            break;
    }

    combine(new);
    return;
}

void transform() {
    ematrix = mat4_mult(tmatrix, ematrix);
    return;
}

void transform_d() {
    dmatrix = mat4_mult(tmatrix, ematrix);
    return;
}

double deg_to_rad(int deg) {
    return (double) ((deg * M_PI) / 180);
}


int isvisible(double p1[3], double p2[3], double p3[3], double eyex, double eyey, double eyez, int parallel){
    int ans = 0;
    double cx, cy, cz, ex, ey, ez; // vectors and cross product (area vector)
    double a, b, c, d, e, f;
    a = p2[0] - p1[0];
    b = p2[1] - p1[1];
    c = p2[2] - p1[2];
    d = p2[0] - p3[0];
    e = p2[1] - p3[1];
    f = p2[2] - p3[2];
    cx = b*f - c*e;
    cy = c*d - a*f;
    cz = a*e - b*d;
    ex = p1[0] - eyex;
    ey = p1[1] - eyey;
    ez = p1[2] - eyez;
    if (!parallel)
        ans = ((p1[0]-eyex) * cx + (p1[1]-eyey) * cy + (p1[2]-eyez) * cz) < 0; // dot prod
    else
        ans = cz < 0; // TODO: Algorithm somehow takes eyes as ABOVE instead of IN FRONT OF


    return ans;
}

