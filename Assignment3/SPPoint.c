#include "SPPoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct sp_point_t {
	int dim;
	int index;
	double* data;
};

/**
 * Allocates a new point in the memory.
 * Given data array, dimension dim and an index.
 * The new point will be P = (p_0,p_2,...,p_{dim-1})
 * such that the following holds
 *
 * - The ith coordinate of the P will be p_i
 * - p_i = data[i]
 * - The index of P = index
 *
 * @return
 * NULL in case data is NULL OR dim <=0 OR index <0
 * Otherwise, the new point is returned
 */
SPPoint spPointCreate(double* data, int dim, int index) {
	SPPoint new_point; //our new point
	int i = 0;

	if (data == NULL || dim <= 0 || index < 0) { //bad args
		return NULL;
	}

	//malloc + check
	if ((new_point = (SPPoint) malloc(sizeof(SPPoint))) == NULL) {
		printf("An error occurred - allocation failure\n");
		return NULL;
	}

	//malloc + check
	if ((new_point->data = (double*) malloc(dim * sizeof(double))) == NULL) {
		printf("An error occurred - allocation failure\n");
		return NULL;
	}

	//fill params into new_point
	new_point->index = index;
	new_point->dim = dim;
	for (; i < dim; i++)
		new_point->data[i] = data[i];

	return new_point;
}

/**
 * Allocates a copy of the given point.
 *
 * Given the point source, the functions returns a
 * new pint P = (P_1,...,P_{dim-1}) such that:
 * - P_i = source_i (The ith coordinate of source and P are the same)
 * - dim(P) = dim(source) (P and source have the same dimension)
 * - index(P) = index(source) (P and source have the same index)
 *
 * @param source - The source point
 * @assert (source != NUlL)
 * @return
 * NULL in case memory allocation occurs
 * Others a copy of source is returned.
 */
SPPoint spPointCopy(SPPoint source) {
	assert(source != NULL);
	return spPointCreate(source->data, source->dim, source->index);
}

/**
 * Free all memory allocation associated with point,
 * if point is NULL nothing happens.
 */
void spPointDestroy(SPPoint point) {
	if (point == NULL) //nothing happens
		return;

	//Free all memory allocation associated with point
	free((point->data));
	free(point);
	return;
}

/**
 * A getter for the dimension of the point
 *
 * @param point - The source point
 * @assert point != NULL
 * @return
 * The dimension of the point
 */
int spPointGetDimension(SPPoint point) {
	assert(point != NULL);
	return point->dim;
}

/**
 * A getter for the index of the point
 *
 * @param point - The source point
 * @assert point != NULL
 * @return
 * The index of the point
 */
int spPointGetIndex(SPPoint point) {
	assert(point != NULL);
	return point->index;
}

/**
 * A getter for specific coordinate value
 *
 * @param point - The source point
 * @param axis  - The coordinate of the point which
 * 				  its value will be retreived
 * @assert point!=NULL && index < dim(point)
 * @return
 * The value of the given coordinate (p_axis will be returned)
 */
double spPointGetAxisCoor(SPPoint point, int axis) {
	assert(point != NULL && axis < spPointGetDimension(point));
	return point->data[axis];
}

/**
 * Calculates the L2-squared distance between p and q.
 * The L2-squared distance is defined as:
 * (p_1 - q_1)^2 + (p_2 - q_1)^2 + ... + (p_dim - q_dim)^2
 *
 * @param p - The first point
 * @param q - The second point
 * @assert p!=NULL AND q!=NULL AND dim(p) == dim(q)
 * @return
 * The L2-Squared distance between p and q
 */
double spPointL2SquaredDistance(SPPoint p, SPPoint q) {
	assert(p != NULL && q != NULL && spPointGetDimension(p) == spPointGetDimension(q));

	int j = 0;
	double dist = 0, temp; //dist will contain result and temp for calculations
	int dim = spPointGetDimension(p); //dim=p.dim

	//calculate L2-squared distance
	for (; j < dim; j++) {
		temp = (double) (p->data[j] - q->data[j]);
		dist = dist + (temp * temp);
	}

	return dist;
}
