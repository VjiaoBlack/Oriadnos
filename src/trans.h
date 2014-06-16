#include "mat4.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

Mat4* ematrix; // sets of 4 columns: define rectangles to draw.
Mat4* dmatrix; // draw matrix.
Mat4* tmatrix;

int* images; // 0 is wall 1 is floor
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
void move(double,double,double);
void scale(double, double, double);
void rotate(char, int);
void move_d(double,double,double);
void scale_d(double, double, double);
void rotate_d(char, int);
void combine(Mat4*);
void transform();
void transform_d();
double deg_to_rad(int);

void push(Mat4*);
Mat4* pop();

int searchtrans(char* name);
void save(Mat4* mat, char* name);
void restore(char* name);
int isvisible(double p1[3], double p2[3], double p3[3], double eyex, double eyey, double eyez, int parallel);
