#include "mat4.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

Mat4* ematrix; // sets of 4 columns: define rectangles to draw.
Mat4* dmatrix; // draw matrix.
Mat4* tmatrix;

#define WALL 0
#define FLOOR 1

int* images;
int numimages;

Mat4** tstack;
int stacksize;

// this will be an array that matches index positions of tmatricies to index position of their names.
// this is also an example of data orientated programming
Mat4** namedtdict;
int namedsize;

char** storagenames;

int xleft, xright, ybottom, ytop; // of input screen
int cleft, rbottom, cright, rtop; // of output pixel file


void init();
Mat4* identity();
void move(float,float,float);
void scale(float, float, float);
void rotate(char, int);
void move_d(float,float,float);
void scale_d(float, float, float);
void rotate_d(char, int);
void combine(Mat4*);
void transform();
void transform_d();
float deg_to_rad(int);

void push(Mat4*);
Mat4* pop();

int searchtrans(char* name);
void save(Mat4* mat, char* name);
void restore(char* name);
int isvisible(float p1[3], float p2[3], float p3[3], float eyex, float eyey, float eyez, int parallel);
