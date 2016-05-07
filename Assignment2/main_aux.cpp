#include "main_aux.h"

/*
 * frees mat[nRows][xlen][ylen] whom init by malloc
 */
void freeMat(int*** data, int xlen, int ylen) {
	for (int i = 0; i < xlen; ++i) {
		if (data[i] != NULL) {
			for (int j = 0; j < ylen; ++j) {
				if (data[i][j] != NULL)
					free(data[i][j]);
			}
			free(data[i]);
		}
	}
	free(data);
}

/*
 * frees mat[nRows][xlen][sizesArray[i]] whom init by malloc
 * sizesArray[i] = nFeatures for the i'th image
 */
void freeDMat(double*** data, int xlen, int* sizesArray) {
	for (int i = 0; i < xlen; ++i) {
		if (data[i] != NULL) {
			for (int j = 0; j < sizesArray[i]; ++j) {
				if (data[i][j] != NULL)
					free(data[i][j]);
			}
			free(data[i]);
		}
	}
	free(data);
}

/**
 * a compare function for qsort()
 */
int cmpfunc(const void * a, const void * b) {
	return (*(int*) b - *(int*) a);
}

/*
 * inits new int mat[xlen][ylen][zlen] using malloc

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
 */
