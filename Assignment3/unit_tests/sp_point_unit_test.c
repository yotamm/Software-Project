#include "../SPPoint.h"
#include "unit_test_util.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MAX_OUTTER_LOOP 100
#define MAX_INNER_LOOP 5
#define DATA_SIZE 128
#define MOD1 2000
#define MOD2 777

//Checks if copy Works
bool pointBasicCopyTest() {
	double data[2] = { 1.0, 1.0 };
	int dim = 2;
	int index = 1;
	int i = 0;
	SPPoint p = spPointCreate(data, dim, index);
	SPPoint q = spPointCopy(p);
	ASSERT_TRUE(spPointGetIndex(p) == spPointGetIndex(q));
	ASSERT_TRUE(spPointGetDimension(p) == spPointGetDimension(q));
	for (; i < spPointGetDimension(p); i++) {
		ASSERT_TRUE(spPointGetAxisCoor(p, i) == spPointGetAxisCoor(q, i));
	}
	spPointDestroy(p);
	spPointDestroy(q);
	return true;
}
bool pointBasicL2Distance() {
	double data1[2] = { 1.0, 1.0 };
	double data2[2] = { 1.0, 0.0 };
	int dim1 = 2;
	int dim2 = 2;
	int index1 = 1;
	int index2 = 1;
	SPPoint p = spPointCreate((double *) data1, dim1, index1);
	SPPoint q = spPointCreate((double *) data2, dim2, index2);
	ASSERT_TRUE(spPointL2SquaredDistance(p, p) == 0.0);
	ASSERT_TRUE(spPointL2SquaredDistance(q, q) == 0.0);
	ASSERT_FALSE(spPointL2SquaredDistance(p, q) == 0.0);
	spPointDestroy(p);
	spPointDestroy(q);
	return true;
}
/*
 * myPointTest() :
 * Checks if create, copy, destroy, getIndex,
 * getAxisCoor, getDimension, destroy, L2SquaredDistance works
 *
 * @param
 * @return true if all works as should, false otherwise
 */
bool myPointTest() {
	SPPoint p, q, z; // p original, q copy of p, and z's data all zeros
	int indexp, i = 0, j, k, sumDist, indexz = 0;
	int dimp = DATA_SIZE, dimz = DATA_SIZE;
	double datap[DATA_SIZE], dataz[DATA_SIZE];

	for (; i <= MAX_OUTTER_LOOP; i++) {
		//fill data with rdm numbers within [0,1,...,MOD1 - 1]
		srand(time(NULL));
		j = 0;
		sumDist = 0;
		for (; j < DATA_SIZE; j++) {
			datap[j] = rand() % MOD1;
			sumDist += (datap[j] * datap[j]);
			dataz[j] = 0;
		}
		//fill indexp with rdm number within [0,1,..,MOD2 - 1]
		indexp = rand() % MOD2;

		p = spPointCreate((double *) datap, dimp, indexp);
		q = spPointCopy(p);
		z = spPointCreate((double *) dataz, dimz, indexz);

		//checking if p and q index and dist and dim are equals
		ASSERT_TRUE(spPointGetIndex(p) == spPointGetIndex(q));
		ASSERT_TRUE(spPointGetDimension(p) == spPointGetDimension(q));
		ASSERT_TRUE(spPointL2SquaredDistance(p, p) == 0.0);
		ASSERT_TRUE(spPointL2SquaredDistance(q, q) == 0.0);
		ASSERT_TRUE(spPointL2SquaredDistance(p, q) == 0.0);

		//checking if L2SquaredDistance = sumDist
		// (not checking if it's not zero because it might happen once in a...)
		ASSERT_TRUE(spPointL2SquaredDistance(p, z) == sumDist);
		ASSERT_TRUE(spPointL2SquaredDistance(q, z) == sumDist);

		//checking if each axis in p and q is equal
		k = 0;
		for (; k < spPointGetDimension(p); k++) {
			ASSERT_TRUE(spPointGetAxisCoor(p, k) == spPointGetAxisCoor(q, k));
		}

		//should destory p, q and z on first time and then should not crash
		j = 0;
		for (; j <= MAX_INNER_LOOP; j++) {
			spPointDestroy(p);
			spPointDestroy(q);
			spPointDestroy(z);
		}
	}
	return true;
}

int main() {
	RUN_TEST(pointBasicCopyTest);
	RUN_TEST(pointBasicL2Distance);
	RUN_TEST(myPointTest);
	return 0;
}
