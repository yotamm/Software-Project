#include "main_aux.h"
/*
 * frees int mat[nRows][xlen][ylen] whom init by malloc
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
 * inits new int mat[nRows][nCols][nDep] using malloc
 */
int*** initIMat(int nRows, int nCols, int nDep) {
	int*** theArray;
	theArray = (int***) malloc(nRows*nCols*sizeof(int**));
	if(theArray == NULL){
		printf("An error occurred - allocation failure\n");
		freeMat(theArray, 0, 0);
		return NULL;
	}
	for (int i = 0; i < nRows; i++)
		for(int j=0; j < nCols; j++){
			theArray[i][j] = (int*) malloc(nDep*sizeof(int));
			if(theArray[i][j] == NULL){
				printf("An error occurred - allocation failure\n");
				freeMat(theArray, i, j);
				return NULL;
			}
		}
	return theArray;
}

