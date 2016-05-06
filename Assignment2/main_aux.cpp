#include "main_aux.h"

/*
 * frees mat[nRows][xlen][ylen] whom init by malloc
 */
void freeMat(int*** data, int xlen, int ylen) {
	for (int i=0; i < xlen; ++i){
		if (data[i] != NULL){
			for (int j=0; j < ylen; ++j){
				if(data[i][j] != NULL)
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
	for (int i=0; i < xlen; ++i){
		if (data[i] != NULL){
			for (int j=0; j < sizesArray[i]; ++j){
				if(data[i][j] != NULL)
					free(data[i][j]);
			}
	        free(data[i]);
	    }
	}
	free(data);
}


