#include "sp_image_proc_util.h"

using namespace std;
using namespace cv;

#define THIRD 0.33
#define MAT_HIST_NUM_ROWS 3
#define SIFT_MAT_NUM_COLS 128
#define MAX_RANGE 256

/*
 * frees int mat[nRows][nCols] whom init by malloc
 * @param matrix to free, x index of first cell we did not malloc
 * @return void
 */
void free_int_Mat(int** toFreeMat, int x) {
	for (int i = 0; i < x; i++)
		free(toFreeMat[i]);
	free(toFreeMat);
}

/*
 * frees double mat[nRows][nCols] whom init by malloc
 * @param matrix to free, x index of first cell we did not malloc
 * @return void
 */
void free_double_Mat(double** toFreeMat, int x) {
	for (int i = 0; i < x; i++)
		free(toFreeMat[i]);
	free(toFreeMat);
}

/*
 * inits new int mat[nRows][nCols] using malloc
 * @param indexes of size of rows and cols
 * @return int** with the sizes sent to func
 */
int** alloc_2D_int_mat(size_t xlen, size_t ylen) {
	int **p;
	size_t i;

	if ((p = (int**) malloc(xlen * sizeof *p)) == NULL) {
		printf("An error occurred - allocation failure\n");
		return NULL;
	}

	for (i = 0; i < xlen; ++i)
		p[i] = NULL;

	for (i = 0; i < xlen; ++i)
		if ((p[i] = (int*) malloc(ylen * sizeof *p[i])) == NULL) {
			printf("An error occurred - allocation failure\n");
			free_int_Mat(p, xlen);
			return NULL;
		}
	return p;
}

/*
 * inits new double mat[nRows][nCols] using malloc
 * @param indexes of size of rows and cols
 * @return double** with the sizes sent to func
 */
double** alloc_2D_double_mat(size_t xlen, size_t ylen) {
	double **p;
	size_t i;

	if ((p = (double**) malloc(xlen * sizeof *p)) == NULL) {
		printf("An error occurred - allocation failure\n");
		return NULL;
	}

	for (i = 0; i < xlen; ++i)
		p[i] = NULL;

	for (i = 0; i < xlen; ++i)
		if ((p[i] = (double*) malloc(ylen * sizeof *p[i])) == NULL) {
			printf("An error occurred - allocation failure\n");
			free_double_Mat(p, xlen);
			return NULL;
		}
	return p;
}

/*
 * Calculates the RGB channels histogram. The histogram will be stored in a
 * two dimensional array of dimensions 3XnBins . The first row is the
 * red channel histogram, the second row is the green channel histogram
 * and the third row is the blue channel histogram.
 *
 * @param str - The path of the image for which the histogram will be calculated
 * @param nBins - The number of subdivision for the intensity histogram
 * @return NULL if str is NULL or nBins <= 0 or allocation error occurred,
 *  otherwise a two dimensional array representing the histogram.
 */
int** spGetRGBHist(char* str, int nBins) {
	Mat src, b_hist, g_hist, r_hist;
	vector<Mat> bgr_planes;
	float range[] = { 0, MAX_RANGE }; // Set the ranges (for B,G,R))
	const float* histRange = { range };
	int histSize = nBins; // Establish the number of bins
	int** res_mat;

	/// Load image
	src = imread(str, CV_LOAD_IMAGE_COLOR);
	if (src.empty() || nBins <= 0)
		return NULL;

	/// Separate the image in 3 places (B, G and R)
	split(src, bgr_planes);

	/// Compute the histograms:
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange);

	//merging B G and R into res_mat
	res_mat = alloc_2D_int_mat(MAT_HIST_NUM_ROWS, nBins);
	if (res_mat == NULL) { //malloc failed
		return NULL;
	}

	//adjust res_mat
	for (int j = 0; j < nBins; j++) {
		//run on the j'th column
		res_mat[0][j] = b_hist.at<float>(0, j);
		res_mat[1][j] = g_hist.at<float>(0, j);
		res_mat[2][j] = r_hist.at<float>(0, j);
	}

	return res_mat;
}

/**
 * Returns the average L2-squared distance between histA and histB. The
 * histA and histB are histograms of the RGB channels. Both histograms
 * must be in the same dimension (3 X nBins).
 * @param histA - RGB histogram of image A
 * @param histB - RGB histogram of image B
 * @return -1 if nBins <= 0 or histA/histB is null, otherwise the average L@-squared distance.
 */
double spRGBHistL2Distance(int** histA, int** histB, int nBins) {
	double avg_dist = 0, temp;

	if (nBins <= 0 || histA == NULL || histB == NULL)
		return -1;
	for (int i = 0; i < MAT_HIST_NUM_ROWS; i++) {
		for (int j = 0; j < nBins; j++) {
			temp = (double) (histA[i][j] - histB[i][j]);
			avg_dist = avg_dist + (temp * temp);
		}
	}
	return (THIRD * avg_dist);
}

/**
 * Extracts AT MOST maxNFeatures SIFT descriptors from the image given by str.
 * The result will be stored in two dimensional matrix with nFeatures
 * rows and 128 columns. Each row represents a SIFT feature corresponding
 * to some keypoint.
 *
 * @param str - A string representing the path of the image
 * @param maxNFeautres - The max number of features features to retain
 * @param nFeatures - A pointer in which the actual number of features retained
 * will be stored.
 * @return NULL in case of:
 * 		   - str is NULL
 * 		   - the image given by str didn't open
 * 		   - nFeatures is NULL
 * 		   - maxNFeatures <= 0
 * 		   - Memory allocation failure
 * 		   otherwise, the total number of features retained will be stored in
 * 		   nFeatures, and the actual features will be returned.
 */
double** spGetSiftDescriptors(char* str, int maxNFeautres, int *nFeatures) {
	double** res_mat;
	Mat image;
	vector<cv::KeyPoint> kp1; //Key points will be stored in kp1
	Mat ds1; //Feature values will be stored in ds1
	Ptr<xfeatures2d::SiftDescriptorExtractor> detect; //Creating  a Sift Descriptor extractor
	Size s;

	if (str == NULL || nFeatures == NULL || maxNFeautres <= 0) //bad args
		return NULL;

	/// Load image
	image = imread(str, CV_LOAD_IMAGE_GRAYSCALE);
	if (image.empty()) //image didnt open
		return NULL;

	detect = xfeatures2d::SIFT::create(maxNFeautres);
	//Extracting features
	//The features will be stored in ds1
	//The output type of ds1 is CV_32F (float)
	detect->detect(image, kp1, Mat());
	detect->compute(image, kp1, ds1);
	s = ds1.size();
	*nFeatures = s.height; //update nFeatures to be the actual number of desc we took

	//init res_mat
	res_mat = alloc_2D_double_mat(*nFeatures, SIFT_MAT_NUM_COLS);
	if (res_mat == NULL) { //malloc failed
		return NULL;
	}

	//adjust res_mat
	for (int i = 0; i < *nFeatures; i++)
		for (int j = 0; j < SIFT_MAT_NUM_COLS; j++)
			res_mat[i][j] = ds1.at<float>(i, j);

	return res_mat;
}

/**
 * Calculates the L2-Square distance of the two features: featureA & featureB
 *
 * @param featureA - The first SIFT feature
 * @param featureB - The second SIFT feature
 * @return -1 in case featureA or featureB is NULL, otherwise the L2-Squared distance
 * as given in the assignment instructions
 */
double spL2SquaredDistance(double* featureA, double* featureB) {
	double dist = 0, temp;
	if (featureA == NULL || featureB == NULL)
		return -1;
	for (int j = 0; j < SIFT_MAT_NUM_COLS; j++) {
		temp = (double) (featureA[j] - featureB[j]);
		dist = dist + (temp * temp);
	}
	return dist;
}

/*int compareIndexed(const void * elem1, const void * elem2) {
	indexedDist *i1, *i2;
	i1 = (indexedDist*) elem1;
	i2 = (indexedDist*) elem2;
	return i1->val - i2->val;
}*/

/*
 * compares two indexedDist based on distance as primary
 * and if dist is equal returns the one with
 * lower index
 */
int my_comparator(const void * elem1, const void * elem2){
	struct indexedDist* x = (indexedDist*) elem1;
	struct indexedDist* y = (indexedDist*) elem2;

	if(x->val == y->val)
		return (x->index - y->index);
	return (((x->val - y->val) > 0) ? 1 : -1);
}

/**
 * Given sift descriptors of the images in the database (databaseFeatures), finds the
 * closest bestNFeatures to a given SIFT feature (featureA). The function returns the
 * INDEXES of the images to which the closest features belong, stored in ascending order
 * (Closest feature image index is first, second closest feature image index is second, etc...).
 * Assumptions:
 *   - Tie break - In case featureA has the same distance (L2Squared distance) from two features,
 *     then the feature that corresponds to the smallest image
 *     index in the database is closer.
 *
 *   - The returned result may contain duplicates in case two features belongs to the same image.
 *
 *   - databaseFeatures is an array of two dimensional arrays, the number of elements
 *     in databaseFeatures is numberOfImages.
 *
 *   - Each entry in databaseFeatures corresponds to the features of some image in the database.
 *     The ith entry corresponds to the features of image_i in the database, and it is a two dimensional
 *     array of dimension nFeaturesPerImage[i]X128.
 *
 *   - The number of descriptors for the ith image is nFeaturesPerImage[i]
 *
 * @param bestNFeatures     - The number of indexes to return.
 * @param featureA          - A sift descriptor which will be compared with the other descriptor
 * 							  (Assumption dimension(bestNFeatures) = 128)
 * @param databaseFeatures  - An array of two dimensional array, in which the descriptors of the images are stored.
 * 							  The ith entry of the array corresponds to the features of the ith image in the database
 * @param numberOfImages    - The number of images in the database. (Number of entries in databaseFeatures)
 * @param nFeaturesPerImage - The number of features for each image. (i.e databaseFeatures[i] is two dimensional
 * 							  array with the dimension nFeaturesPerImage[i]X128
 * @return - NULL if either the following:
 * 			 * featureA is NULL
 * 			 * databaseFeatures is NULL
 * 			 * numberOfImages <= 1
 * 			 * nFeaturesPerImage is NULL
 * 			 * allocation error occurred
 * 			 otherwise, an array of size bestNFeatures is returned such that:
 * 			 * Given that f1, f2, ... the closest features to featureA (i.e d(featureA,f1) <= d(featureA,f2) <= ...)
 * 			 * i1, i2, .... are the indexes of the images to which fi belongs (i.e f1 is a SIFT descriptor of image i1,
 * 			   f2 is a SIFT descriptor of image i2 etc..)
 * 			 Then the array returned is {i1,i2,...,i_bestNFeatures}
 */

int* spBestSIFTL2SquaredDistance(int bestNFeatures, double* featureA,
		double*** databaseFeatures, int numberOfImages,
		int* nFeaturesPerImage) {
	struct indexedDist bestFeaturesDist[numberOfImages];
	double min, current;
	int* result;
	if ((result = (int*) malloc(bestNFeatures * sizeof(int))) == NULL) {
		printf("An error occurred - allocation failure\n");
		return NULL;
	}
	//Check for NULL
	if (featureA == NULL || databaseFeatures == NULL
			|| numberOfImages <= 1|| nFeaturesPerImage==NULL) {
		return NULL;
	}
	//find best matching features from each image
	for (int i = 0; i < numberOfImages; i++) {
		min = spL2SquaredDistance(databaseFeatures[i][0], featureA);
		for (int j = 1; j < nFeaturesPerImage[i]; j++) {
			current = spL2SquaredDistance(databaseFeatures[i][j], featureA);
			if (current < min) {
				min = current;
			}
		}
		bestFeaturesDist[i].val = min;
		bestFeaturesDist[i].index = i;
	}
	//sort the best features and fill the result array
	qsort(bestFeaturesDist, numberOfImages, sizeof(struct indexedDist),
			&my_comparator);
	for (int k = 0; k < bestNFeatures; k++) {
		result[k] = bestFeaturesDist[k].index;
	}
	return result;
}

/**
 * Given an RGB histogram array and an RGB histogram. Calculate the best L2Square distances,
 * and return the indices of numBest lowest distances.
 *
 * @param histArray - Array of RGB histograms
 * @param numImages - histArray length
 * @param numBins   - number of bins in the histograms
 * @param queryHist - histogram to compare to
 * @param numBest   - number of closest images to return
 * @return          - the indices of the numBest closest L2Sqaure images from histArray
 */
int* spBestRGBHistL2SquareDistance(int*** histArray, int numImages, int numBins,
		int** queryHist, int numBest) {
	struct indexedDist bestHistDist[numImages];
	//double min, current;
	int* result;

	if ((result = (int*) malloc(numBest * sizeof(int))) == NULL) {
		printf("An error occurred - allocation failure\n");
		return NULL;
	}

	//Check for NULL and bad args
	if (queryHist == NULL || histArray == NULL || numImages <= 1 || numBins <= 0
			|| numBest < 1) {
		return NULL;
	}

	//calculate
	for (int i = 0; i < numImages; i++) {
		bestHistDist[i].val = spRGBHistL2Distance(histArray[i], queryHist,
				numBins);
		bestHistDist[i].index = i;
	}

	//sort the best features and fill the result array
	qsort(bestHistDist, numImages, sizeof(struct indexedDist), &my_comparator);
	for (int k = 0; k < numBest; k++) {
		result[k] = bestHistDist[k].index;
	}
	return result;
}

