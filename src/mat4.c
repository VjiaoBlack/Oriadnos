/*	Basic functions for 4-row matrices */
#include <stdio.h>
#include <stdlib.h>
#include "mat4.h"


/* 	==================== mat4_create() ========================================================
	Create a matrix (with 4 rows) with ncolumns, where ncolumns >= 0.  
	If ncolumns > 0, then set all cells to 0.
	Returns pointer to Mat4.
	
	Usage: Mat4 *a = mat4_create(10);  // creates a 4 x 10 matrix, zero-filled
*/
Mat4 *mat4_create(int ncolumns) {
	Mat4 *result;
	
	if (ncolumns < 0) {
		printf("Error in mat4_create().  ncolumns must be >= 0\n");
		return NULL;
	}
	
	result = calloc(1,sizeof(Mat4));
	if (ncolumns == 0)
		return result;
	
	if ((result->cells = calloc(ncolumns*4,sizeof(double))) == NULL) {
		printf("Error in mat4_create(): calloc failed on ncolumns == %d\n", ncolumns);
		return NULL;
	}
	
	result->cols = ncolumns;
	return result;
}

/* ===================== mat4_delete() ===================================================
	Delete a previously created mat4 matrix.
	
	Usage: mat4_delete(a);
*/

void mat4_delete(Mat4 *m) {
	if (m->cols > 0)
		free(m->cells);
	free(m);
}

/* ======================  mat4_create_identity()  ====================================================
	Will create an 4x4 identity matrix.
	Returns pointer to Mat4.
	
	Usage:  Mat4 *a = mat4_create_identity()
*/
Mat4 *mat4_create_identity() {
	int i;
	Mat4 *m = mat4_create(4);
	for (i = 0; i < 4; ++i)
		mat4_set(m, i, i, 1);
	return m;
}


/* ============================== mat4_copy() ============================================================
    Create a copy of a matrix
    
    Usage: Mat4 *c = mat4_copy(a);
*/
Mat4 *mat4_copy(Mat4 *original) {
    int row, col;
    Mat4 *result = mat4_create(original->cols);
    if (original->cols > 0) {
        for (row = 0; row < 4; ++row) {
            for (col = 0; col < original->cols; ++col) {
                mat4_set(result,row,col,mat4_get(original,row,col));
            }
        }
    }
    return result;
}

/* =============================== mat4_add_column() ====================================================
	Adds a column to the right-hand end of an existent matrix.
	Returns the modified matrix (old matrix is modified).
	
	Usage:  
		double new_cells[4] = {1.0, 2.3, 4.0, 1.0};
		Mat4 *new = mat4_add_column(old, new_cells);
*/
Mat4 *mat4_add_column(Mat4 *old, double *new_cells) {
	int i;
	
	if (old->cols == 0)
		old->cells = calloc(4, sizeof(double));
	else 
		old->cells = realloc(old->cells,(old->cols+1) * 4 * sizeof(double));
	old->cols += 1;
	for (i = 0; i < 4; ++i)
		mat4_set(old, i, old->cols-1, new_cells[i]);
	return old;
}

/* ============================== mat4_set() =========================================================
	Sets a cell in the matrix to a value
	
	Usage:
		mat4_set(m, row, col, new_value);
		NOTE: row and col indices start at 0.
		Returns m;
*/
Mat4 *mat4_set(Mat4 *matrix, int row, int col, double value) {
	if (row < 0 || row >= 4 || col < 0 || col >= matrix->cols) {
		printf("Error in mat4_set(), index out of range, row = %d, col = %d\n",row,col);
		return matrix;
	}
	matrix->cells[col*4+row] = value;
	return matrix;
}

/* =============================== mat4_get() ===========================================================
	Returns the value in a cell.
	
	Usage:
		double c = mat4_get(m, row, col);
		NOTE: row and col indices start at 0.
*/
double mat4_get(Mat4 *matrix, int row, int col) {
	if (row < 0 || row >= 4 || col < 0 || col >= matrix->cols) {
		printf("Error in mat4_get(), index out of range, row = %d, col = %d\n",row,col);
		return 0.0;
	}
    // printf("getting: %d, %d\n", row, col);
	return matrix->cells[col*4+row];
}

double dotprod(double* u, double* v) {
	int i;
    double result = 0;
	for (i = 0; i < 4; i++) {
		result += u[i] * v[i];
	}
	return result;
}

Mat4* mat4_mult(Mat4* tmatrix, Mat4* imatrix) {
	Mat4* result = mat4_create(imatrix->cols);

	double* trow = malloc(sizeof(double) * 4);

	double* icol = malloc(sizeof(double) * 4);

    int c = 0, r = 0;
    while (r < 4) {
        while (c < result->cols) {
        	trow[0] = mat4_get(tmatrix,r,0);
        	trow[1] = mat4_get(tmatrix,r,1);
        	trow[2] = mat4_get(tmatrix,r,2);
        	trow[3] = mat4_get(tmatrix,r,3);
        	icol[0] = mat4_get(imatrix,0,c);
        	icol[1] = mat4_get(imatrix,1,c);
        	icol[2] = mat4_get(imatrix,2,c);
        	icol[3] = mat4_get(imatrix,3,c);
            mat4_set(result, r, c, dotprod(trow,icol));
            c++;
        }
        r++;
        c = 0;
    }
    return result;
}

/* ================================= mat4_columns ===================================================
    Returns the number of columns in a matrix.
    
    Usage: int n = mat4_columns(m);
*/
int mat4_columns(Mat4 *m) {
    return m->cols;
}

void print(Mat4* matrix) {
    // going through the matrix by rows
    int c = 0, r = 0;
    while (r < 4) {
        while (c < matrix->cols) {
            printf("%5.2f ", mat4_get(matrix, r, c));
            c++;
        }
        r++;
        c = 0;
        printf("\n");
    }
    return;
}


	
