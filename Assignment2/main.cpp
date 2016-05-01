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

int main() {
	//init vars
	char s_num_imgs[1025], dir[1025], pref[1025], suff[1025], full_img_url[1025];
	int num_imgs = 0, num_bins = 0, max_num_sift = 0;
	int*** histArray; //3D Matrix that will contain the images histograms
	double*** descArray; ////3D Matrix that will contain the images descriptors
	int* nFeatures;

	//stage 1
	printf("Enter images directory path:\n");
	fflush(NULL);
	scanf("%s",dir);

	//stage 2
	printf("Enter images prefix:\n");
	fflush(NULL);
	scanf("%s",pref);

	//stage 3
	printf("Enter number of images:\n");
	fflush(NULL);
	scanf("%s",s_num_imgs);
	num_imgs = atoi(s_num_imgs);
	if(num_imgs < 1){ //bad arg
		printf("An error occurred - invalid number of images\n");
		//nothing to free
		return -1;
	}

	//stage 4
	printf("Enter images suffix:\n");
	fflush(NULL);
	scanf("%s",suff);

	//stage 5
	printf("Enter number of bins:\n");
	fflush(NULL);
	scanf("%d",&num_bins);
	if(num_bins < 1){ //bad arg
		printf("An error occurred - invalid number of bins\n");
		//nothing to free
		return -1;
	}

	//stage 6
	printf("Enter number of features:\n");
	fflush(NULL);
	scanf("%d",&max_num_sift);
	if(max_num_sift < 1){ //bad arg
		printf("An error occurred - invalid number of features\n");
		//nothing to free
		return -1;
	}

	//stage 7
	//init RGB histogram for each image in the images directory
	histArray = alloc_3D_int_mat(num_imgs, THREE_FOR_RGB, num_bins);
	if(histArray == NULL){ //malloc failed
		 return -1;
	}

	descArray = alloc_3D_double_mat(num_imgs, MAT_NUM_COLS, max_num_sift);
	if(descArray == NULL){ //malloc failed
		freeMat(histArray, 0, 0); //free histArray
		return -1;
	}

	//calculate histogram and descriptors  for each image
	nFeatures = (int*)malloc(sizeof nFeatures);
	for (int i = 0; i < num_imgs; i++){
		//concat the image full url
		sprintf(full_img_url, "%s%s%d%s", dir, pref, i, suff);

		//calc hist for the i'th img
		histArray[i] = spGetRGBHist((char*)full_img_url, num_bins);

		//calc desc for the i'th img
		descArray[i] = spGetSiftDescriptors((char*)full_img_url, max_num_sift, nFeatures);

		if(histArray[i] == NULL || descArray[i] == NULL){
			//wrong url or nBins <= 0 or malloc failed
			freeMat(histArray, i, THREE_FOR_RGB); //free histArray
			freeDMat(descArray, i, (*nFeatures)); //free descArray
			return -1;
		}
	}

	return 0;
}
