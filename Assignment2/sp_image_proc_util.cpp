#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#define THIRD 0.33
#define MAT_NUM_COLS 128

/*
 * inits new int mat[nRows][nCols] using malloc
 */
int** initIMat(int nRows, int nCols) {
	//~~~tested and works~~~
	int** theArray;
	theArray = (int**) malloc(nRows*sizeof(int*));
	for (int i = 0; i < nRows; i++)
		theArray[i] = (int*) malloc(nCols*sizeof(int));
	return theArray;
}

/*
 * inits new double mat[nRows][nCols] using malloc
 */
double** initDMat(int nRows, int nCols) {
	//~~~tested and works~~~
	double** theArray;
	theArray = (double**) malloc(nRows*sizeof(double*));
	for (int i = 0; i < nRows; i++)
		theArray[i] = (double*) malloc(nCols*sizeof(double));
	return theArray;
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
int** spGetRGBHist(char* str, int nBins){
	//~~~should work, almost all from ppt~~
	  Mat src;
	  /// Load image
	  src = imread(str, CV_LOAD_IMAGE_COLOR);
	  if(src.empty() || nBins <= 0)
	     return NULL;

	  /// Separate the image in 3 places (B, G and R)
	  vector<Mat> bgr_planes;
	  split(src, bgr_planes);

	  /// Establish the number of bins
	  int histSize = nBins;

	  /// Set the ranges (for B,G,R))
	  float range[] = {0, 256};
	  const float* histRange = {range};

	  Mat b_hist, g_hist, r_hist;

	  /// Compute the histograms:
	  calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange);
	  calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange);
	  calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange);

	  //merging B G and R into res_mat
	  int** res_mat = initIMat(3, nBins);
	  if(res_mat == NULL) //malloc failed;
		  return NULL;

	  for(int j = 0; j < nBins; j++){
		  res_mat[0][j] = b_hist.at<float>(0,j);
		  res_mat[1][j] = g_hist.at<float>(0,j);
		  res_mat[2][j] = r_hist.at<float>(0,j);
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
double spRGBHistL2Distance(int** histA, int** histB, int nBins){
	double avg_dist = 0;
	if(nBins <= 0 || histA == NULL || histB == NULL)
		return -1;
	for(int j = 0; j < nBins; j++){
		//calc on column j
		avg_dist = avg_dist + ((histA[0][j] - histB[0][j])^2);
		avg_dist = avg_dist + ((histA[1][j] - histB[1][j])^2);
		avg_dist = avg_dist + ((histA[2][j] - histB[2][j])^2);
	}
	return (THIRD*avg_dist);
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
double** spGetSiftDescriptors(char* str, int maxNFeautres, int *nFeatures){
	//~~~tested and works~~~

	Mat image;
	if(str == NULL || nFeatures == NULL || maxNFeautres <= 0) //bad args
		return NULL;
	*nFeatures = (*nFeatures > maxNFeautres ? maxNFeautres : *nFeatures);

	/// Load image
	image = imread(str, CV_LOAD_IMAGE_COLOR);
	if(image.empty()) //image didnt open
		return NULL;

	//init res_mat
	double** res_mat = initDMat(*nFeatures, MAT_NUM_COLS);
	if(res_mat == NULL) //malloc failed
		return NULL;

	//Calculate descriptors
	vector<KeyPoint> keypoints;
	Ptr<BRISK> det_Brisk = BRISK::create();
	det_Brisk->detect(image, keypoints);
	delete det_Brisk;

	Ptr<BRISK> ext_Brisk = BRISK::create();
	Mat descriptors;
	ext_Brisk->compute(image, keypoints, descriptors);
	delete ext_Brisk;

	//adjust res_mat
	for(int i=0; i<*nFeatures; i++)
		for(int j=0; j< MAT_NUM_COLS; j++)
			res_mat[i][j] = descriptors.at<double>(i,j);

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
double spL2SquaredDistance(double* featureA, double* featureB){
	double dist = 0;
	if(featureA == NULL || featureB == NULL)
		return -1;
	for(int j = 0; j < 128; j++){
		dist = dist + (featureA[j] - featureB[j])*(featureA[j] - featureB[j]);
	}
	return dist;
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
		int* nFeaturesPerImage);



