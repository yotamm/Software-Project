#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include "sp_image_proc_util.h"
#include "main_aux.h"

using namespace cv;
using namespace std;

#define THREE_FOR_RGB 3
#define MAT_NUM_COLS 128
#define FIVE 5

int main() {
	//init vars
	char s_num_imgs[1025], dir[1025], pref[1025], suff[1025],
			full_img_url[1025], query_url[1025];
	int num_imgs = 0, num_bins = 0, max_num_sift = 0;
	int*** histArray; //3D Matrix that will contain the images histograms
	double*** descArray; ////3D Matrix that will contain the images descriptors
	int* nFeatures;
	int* sizesArray; //this array will contain in i'th cell nFeatures of i'th img
	struct indexedDist* bestSIFTHits;
	int** queryHist;
	double** querySIFT;
	int* closestHist;
	int* closestSIFT;

	//stage 1
	printf("Enter images directory path:\n");
	fflush(NULL);
	scanf("%s", dir);

	//stage 2
	printf("Enter images prefix:\n");
	fflush(NULL);
	scanf("%s", pref);

	//stage 3
	printf("Enter number of images:\n");
	fflush(NULL);
	scanf("%s", s_num_imgs);
	num_imgs = atoi(s_num_imgs);
	if (num_imgs < 1) { //bad arg
		printf("An error occurred - invalid number of images\n");
		//nothing to free
		return -1;
	}

	//stage 4
	printf("Enter images suffix:\n");
	fflush(NULL);
	scanf("%s", suff);

	//stage 5
	printf("Enter number of bins:\n");
	fflush(NULL);
	scanf("%d", &num_bins);
	if (num_bins < 1) { //bad arg
		printf("An error occurred - invalid number of bins\n");
		//nothing to free
		return -1;
	}

	//stage 6
	printf("Enter number of features:\n");
	fflush(NULL);
	scanf("%d", &max_num_sift);
	if (max_num_sift < 1) { //bad arg
		printf("An error occurred - invalid number of features\n");
		//nothing to free
		return -1;
	}

	//stage 7
	//init RGB histogram for each image in the images directory
	histArray = (int***) malloc(num_imgs * sizeof *histArray);
	if (histArray == NULL) { //malloc failed
		printf("An error occurred - allocation failure\n");
		return -1;
	}

	//init descriptor for each image in the images directory
	descArray = (double***) malloc(num_imgs * sizeof *descArray);
	sizesArray = (int*) malloc(num_imgs * sizeof(int));
	if (descArray == NULL || sizesArray == NULL) { //malloc failed
		printf("An error occurred - allocation failure\n");
		freeMat(histArray, 0, 0); //free histArray
		freeDMat(descArray, 0, sizesArray); //free descArray
		free(sizesArray);
		return -1;
	}

	//calculate histogram and descriptors  for each image
	nFeatures = (int*) malloc(sizeof nFeatures);
	for (int i = 0; i < num_imgs; i++) {
		//concat the image full url
		sprintf(full_img_url, "%s%s%d%s", dir, pref, i, suff);

		//calc hist for the i'th img
		histArray[i] = spGetRGBHist((char*) full_img_url, num_bins);

		//calc desc for the i'th img
		descArray[i] = spGetSiftDescriptors((char*) full_img_url, max_num_sift,
				nFeatures);
		sizesArray[i] = *nFeatures;

		if (histArray[i] == NULL || descArray[i] == NULL) {
			//wrong url or nBins <= 0 or malloc failed
			freeMat(histArray, i, THREE_FOR_RGB); //free histArray
			freeDMat(descArray, i, sizesArray); //free descArray
			free(sizesArray);
			free(nFeatures);
			return -1;
		}
	}

	//stages 8 + 9 + 10
	//allocation and failure handling
	bestSIFTHits = (indexedDist*) malloc(num_imgs * sizeof(indexedDist));
	if (bestSIFTHits == NULL) { //malloc failed
		printf("An error occurred - allocation failure\n");
		freeMat(histArray, num_imgs, THREE_FOR_RGB); //free histArray
		freeDMat(descArray, num_imgs, sizesArray); //free descArray
		free(sizesArray);
		free(nFeatures);
		return -1;
	}

	//loop runs until user enters # to terminate
	while (true) {
		//init var
		int k;

		printf("Enter a query image or # to terminate:\n");
		fflush(NULL);
		scanf("%s", query_url);
		if (query_url[0] == '#') {
			//need to terminate
			printf("Exiting...\n");
			freeMat(histArray, num_imgs, THREE_FOR_RGB); //free histArray
			freeDMat(descArray, num_imgs, sizesArray); //free descArray
			free(sizesArray);
			free(nFeatures);
			free(bestSIFTHits);
			return 0;
		}
		for (int h=0; h<num_imgs; h++){
			bestSIFTHits[h].index=h;
			bestSIFTHits[h].val=0;
		}

		//calculate best SIFT hits and RGB hits
		queryHist = spGetRGBHist(query_url, num_bins);
		closestHist = spBestRGBHistL2SquareDistance(histArray, num_imgs,
				num_bins, queryHist, FIVE);
		querySIFT = spGetSiftDescriptors(query_url, max_num_sift,
				nFeatures);
		for (int i = 0; i < max_num_sift; i++) {
			closestSIFT = spBestSIFTL2SquaredDistance(FIVE, querySIFT[i],
					descArray, num_imgs, sizesArray);
			for (int j = 0; j < FIVE; j++) {
				k = closestSIFT[j];
				bestSIFTHits[k].val = bestSIFTHits[k].val + 1;
			}
		}
		qsort(bestSIFTHits, num_imgs, sizeof(indexedDist), &my_aux_comparator);

		//print results for current image
		printf("Nearest images using global descriptors:\n");
		printf("%d, %d, %d, %d, %d\n", closestHist[0], closestHist[1],
				closestHist[2], closestHist[3], closestHist[4]);
		printf("Nearest images using local descriptors:\n");
		printf("%d, %d, %d, %d, %d\n", bestSIFTHits[0].index, bestSIFTHits[1].index,
				bestSIFTHits[2].index, bestSIFTHits[3].index, bestSIFTHits[4].index);

		//free alloc for the current query
		free(closestHist);
		free_int_Mat(queryHist, THREE_FOR_RGB); //free queryHist
		free_double_Mat(querySIFT, THREE_FOR_RGB); //free querySIFT
	}

	//not needed
	//but won't harm :)
	return 0;
}
