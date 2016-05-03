#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

/*
 * inits new int mat[xlen][ylen][zlen] using malloc
 * @param sizes of rows, cols, depth
 * @return 3D mat[xlen][ylen][zlen]
 */
//int*** alloc_3D_int_mat(size_t xlen, size_t ylen, size_t zlen);

/*
 * frees int mat[nRows][xlen][ylen] whom init by malloc
 * @param 3D matrix to be freed, xlen and ylen blocking the indexes who been malloced
 * @return void
 */
void freeMat(int*** data, int xlen, int ylen);

/*
 * frees mat[nRows][xlen][sizesArray[i]] whom init by malloc
 * sizesArray[i] = nFeatures for the i'th image
 * @param 3D matrix to be freed, xlen and ylen blocking the indexes who been malloced
 * @return void
 */
void freeDMat(double*** data, int xlen, int* sizesArray);

/*
 * inits new double mat[xlen][ylen][zlen] using malloc
 * @param sizes of rows, cols, depth
 * @return 3D mat[xlen][ylen][zlen]
 */
//double*** alloc_3D_double_mat(size_t xlen, size_t ylen, size_t zlen);


#endif /* MAIN_AUX_H_ */
