#include <stdlib.h>
#include <string.h>

/*
 * function for concatinating two strings
 * for example :
 * if params are - "abc" and "lmn"
 * @return - "abclmn"
 */
char* concat(char *s1, char *s2)
{
    char* result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    if(result == NULL){ //malloc failed
		printf("An error occurred - allocation failure\n");
   		return NULL;
    }
    strcpy(result, s1);
    strcat(result, s2);
    return result;
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

/*
 * frees int mat[nRows][nCols][nDep] whom init by malloc
 */
void freeMat(int*** toFreeMat, int x, int y) {
	for (int i = 0; i < x; i++)
		for(int j=0; j < y; j++)
			free(toFreeMat[i][j]);
	free(toFreeMat);
}
