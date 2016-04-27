#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include "sp_image_proc_util.h"
#include "main_aux.h"

using namespace cv;
using namespace std;

#define THREE_FOR_RGB 3

int main() {
	//init vars
	char s_num_imgs[1025], dir[1025], pref[1025], suff[1025], full_img_url[1025];
	int num_imgs, num_bins, max_num_sift;
	int*** histArray; //3D Matrix that will contain the images histograms

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
	histArray = initIMat(num_imgs, THREE_FOR_RGB, num_bins);
	if(histArray == NULL){ //malloc failed
		 return -1;
	}

	//calculate histogram for each image
	for (int i = 0; i < num_imgs; i++){
		//concat the image full url
		sprintf(full_img_url, "%s%s%d%s", dir, pref, i, suff);

		//calc hist for the i'th img
		histArray[i] = spGetRGBHist((char*)full_img_url, num_bins);

		if(histArray[i] == NULL){  //wrong url or nBins <= 0 or malloc failed
			freeMat(histArray, i, THREE_FOR_RGB); //free histArray
			return -1;
		}
	}

	return 0;
}
