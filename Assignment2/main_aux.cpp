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

/*
 * Note : cannot change sp_image_proc_util.h
 *
 * compares two indexedDist based on distance as primary
 * and if dist is equal returns the one with
 * lower index
 */
int my_aux_comparator(const void * elem1, const void * elem2){
	struct indexedDist* x = (indexedDist*) elem1;
	struct indexedDist* y = (indexedDist*) elem2;

	if(x->val == y->val)
		return (x->index - y->index);
	return (((x->val - y->val) > 0) ? 1 : -1);
}

/*
 * frees int mat[nRows][nCols] whom init by malloc
 * @param matrix to free, x index of first cell we did not malloc
 * @return void
 */
void free_2Dint_Mat(int** toFreeMat, int x) {
	for (int i = 0; i < x; i++)
		free(toFreeMat[i]);
	free(toFreeMat);
}

/*
 * frees double mat[nRows][nCols] whom init by malloc
 * @param matrix to free, x index of first cell we did not malloc
 * @return void
 */
void free_2Ddouble_Mat(double** toFreeMat, int x) {
	for (int i = 0; i < x; i++)
		free(toFreeMat[i]);
	free(toFreeMat);
}

