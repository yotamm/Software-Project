#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

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
 * Note : cannot change sp_image_proc_util.h
 *
 * compares two indexedDist based on distance as primary
 * and if dist is equal returns the one with
 * lower index
 */
int my_aux_comparator(const void * elem1, const void * elem2);

#endif /* MAIN_AUX_H_ */
