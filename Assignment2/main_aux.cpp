#include "main_aux.h"

/*
 * frees mat[nRows][xlen][ylen] whom init by malloc
 */
void freeMat(int*** data, int xlen, int ylen) {
	for (int i=0; i < xlen; ++i){
		if (data[i] != NULL){
			for (int j=0; j < ylen; ++j)
				free(data[i][j]);
	        free(data[i]);
	    }
	}
	free(data);
}

/*
 * frees mat[nRows][xlen][ylen] whom init by malloc
 */
void freeDMat(double*** data, int xlen, int ylen) {
	for (int i=0; i < xlen; ++i){
		if (data[i] != NULL){
			for (int j=0; j < ylen; ++j)
				free(data[i][j]);
	        free(data[i]);
	    }
	}
	free(data);
}

/*
 * inits new int mat[xlen][ylen][zlen] using malloc
 */
int*** alloc_3D_int_mat(size_t xlen, size_t ylen, size_t zlen)
{
    int ***p;
    size_t i, j;

    if ((p = (int***)malloc(xlen * sizeof *p)) == NULL) {
    	printf("An error occurred - allocation failure\n");
        return NULL;
    }

    for (i=0; i < xlen; ++i)
        p[i] = NULL;

    for (i=0; i < xlen; ++i)
        if ((p[i] = (int**)malloc(ylen * sizeof *p[i])) == NULL) {
        	printf("An error occurred - allocation failure\n");
        	freeMat(p, xlen, ylen);
            return NULL;
        }

    for (i=0; i < xlen; ++i)
        for (j=0; j < ylen; ++j)
            p[i][j] = NULL;

    for (i=0; i < xlen; ++i)
        for (j=0; j < ylen; ++j)
            if ((p[i][j] = (int*)malloc(zlen * sizeof *p[i][j])) == NULL) {
            	printf("An error occurred - allocation failure\n");
            	freeMat(p, xlen, ylen);
                return NULL;
            }

    return p;
}

/*
 * inits new double mat[xlen][ylen][zlen] using malloc
 */
double*** alloc_3D_double_mat(size_t xlen, size_t ylen, size_t zlen)
{
	double*** p;
    size_t i, j;

    if ((p = (double***)malloc(xlen * sizeof *p)) == NULL) {
    	printf("An error occurred - allocation failure\n");
        return NULL;
    }

    for (i=0; i < xlen; ++i)
        p[i] = NULL;

    for (i=0; i < xlen; ++i)
        if ((p[i] = (double**)malloc(ylen * sizeof *p[i])) == NULL) {
        	printf("An error occurred - allocation failure\n");
        	freeDMat(p, xlen, ylen);
            return NULL;
        }

    for (i=0; i < xlen; ++i)
        for (j=0; j < ylen; ++j)
            p[i][j] = NULL;

    for (i=0; i < xlen; ++i)
        for (j=0; j < ylen; ++j)
            if ((p[i][j] = (double*)malloc(zlen * sizeof *p[i][j])) == NULL) {
            	printf("An error occurred - allocation failure\n");
            	freeDMat(p, xlen, ylen);
                return NULL;
            }

    return p;
}

