#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

/*
 * inits new int mat[nRows][nCols][nDep] using malloc
 * @param sizes of rows, cols, depth
 * @return 3D mat[nRows][nCols][nDep]
 */
int*** initIMat(int nRows, int nCols, int nDep);

/*
 * frees int mat[nRows][xlen][ylen] whom init by malloc
 * @param 3D matrix to be freed, xlen and ylen blocking the indexes who been malloced
 * @return void
 */
void freeMat(int*** data, int xlen, int ylen);

#endif /* MAIN_AUX_H_ */
