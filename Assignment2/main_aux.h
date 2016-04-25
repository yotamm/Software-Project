/*
 * main_aux.h
 *
 *  Created on: 25 באפר 2016
 *      Author: varda
 */

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

/*
 * function for concatinating two strings
 * if param are - "abc" and "lmn"
 * @return - "abclmn"
 */
char* concat(char *s1, char *s2);

/*
 * inits new int mat[nRows][nCols][nDep] using malloc
 */
int*** initIMat(int nRows, int nCols, int nDep);

#endif /* MAIN_AUX_H_ */
